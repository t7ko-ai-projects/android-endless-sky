/* TargetPickerPanel.cpp
Copyright (c) 2026 by Ivan Tishchenko

Endless Sky is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

Endless Sky is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program. If not, see <https://www.gnu.org/licenses/>.
*/

#include "TargetPickerPanel.h"

#include "Color.h"
#include "Command.h"
#include "GameData.h"
#include "Government.h"
#include "Personality.h"
#include "PlayerInfo.h"
#include "Rectangle.h"
#include "Screen.h"
#include "Ship.h"
#include "UI.h"
#include "System.h"
#include "shader/FillShader.h"
#include "shader/LineShader.h"
#include "shader/SpriteShader.h"
#include "image/Sprite.h"
#include "text/DisplayText.h"
#include "text/Font.h"
#include "text/FontSet.h"
#include "text/Format.h"
#include "text/Layout.h"
#include "text/Truncate.h"

#include <algorithm>
#include <cmath>
#include <set>
#include <string>

using namespace std;

namespace {
	const double CELL_WIDTH = 250.;
	const double CELL_HEIGHT = 120.;
	const double GAP = 10.;
	const double TOP_MARGIN = 50.;
	const double ICON_SIZE = 80.;
	const double TEXT_WIDTH = CELL_WIDTH - ICON_SIZE - 20.;
	const double TOGGLE_WIDTH = 150.;
	const double TOGGLE_HEIGHT = 30.;
	// About half a second at 60 frames per second.
	const int LONG_PRESS_STEPS = 30;

	// Whether the player's own fleet, friendly ships and hidden ships are listed. All start
	// off and are remembered until the game is restarted.
	bool showFleet = false;
	bool showFriendly = false;
	bool showHidden = false;
	const int TOGGLES = 3;

	// Ships and models the player hid. Forgotten when the flagship leaves the system.
	const System *hiddenSystem = nullptr;
	vector<weak_ptr<Ship>> hiddenShips;
	set<string> hiddenModels;
	const double CROSS_SIZE = 14.;
	const double CROSS_RANGE = 30.;

	bool IsHiddenShip(const Ship *ship)
	{
		for(const weak_ptr<Ship> &it : hiddenShips)
			if(it.lock().get() == ship)
				return true;
		return false;
	}

	bool IsHidden(const Ship &ship)
	{
		return IsHiddenShip(&ship) || hiddenModels.contains(ship.TrueModelName());
	}

	// Hidden ships and models only apply in the system they were hidden in.
	void ForgetHiddenIfMoved(const Ship &flagship)
	{
		if(hiddenSystem != flagship.GetSystem())
		{
			hiddenSystem = flagship.GetSystem();
			hiddenShips.clear();
			hiddenModels.clear();
		}
	}

	// Whether a ship in this system can be targeted at all.
	bool IsCandidate(const Ship &ship, const Ship &flagship)
	{
		return &ship != &flagship && ship.GetSystem() == flagship.GetSystem() && ship.IsTargetable();
	}

	// Whether the toggles let this ship into the grid.
	bool IsListed(const Ship &ship)
	{
		if(ship.IsYours() ? !showFleet
				: !(ship.GetGovernment() && ship.GetGovernment()->IsEnemy()) && !showFriendly)
			return false;
		return showHidden || !IsHidden(ship);
	}

	void DrawCross(const Point &center, const Color &color)
	{
		const Point d(.5 * CROSS_SIZE, .5 * CROSS_SIZE);
		LineShader::Draw(center - d, center + d, 2.5f, color);
		LineShader::Draw(center + Point(-d.X(), d.Y()), center + Point(d.X(), -d.Y()), 2.5f, color);
	}

	// Your own ships and friendly escorts, which the target key cycles through with Shift.
	bool IsFriendlyEscort(const Ship &ship)
	{
		return ship.IsYours() || (ship.GetPersonality().IsEscort() && !ship.GetGovernment()->IsEnemy());
	}

	// The same colors the game uses for the target outline.
	const Color &RelationColor(const Ship &ship)
	{
		if(ship.IsYours())
			return *GameData::Colors().Get("ship target outline player");
		if(ship.IsDisabled())
			return *GameData::Colors().Get("ship target outline inactive");
		if(ship.GetGovernment() && ship.GetGovernment()->IsEnemy())
			return *GameData::Colors().Get("ship target outline hostile");
		return *GameData::Colors().Get("ship target outline friendly");
	}
}



TargetPickerPanel::TargetPickerPanel(PlayerInfo &player, const list<shared_ptr<Ship>> &ships)
	: player(player)
{
	// Back closes this panel instead of opening the main menu.
	SetInterruptible(false);
	const Ship *flagship = player.Flagship();
	if(!flagship)
		return;
	ForgetHiddenIfMoved(*flagship);

	vector<shared_ptr<Ship>> found;
	for(const shared_ptr<Ship> &ship : ships)
		if(IsCandidate(*ship, *flagship))
			found.push_back(ship);
	// Disabled ships first, then the rest; nearest first within each.
	const Point here = flagship->Position();
	sort(found.begin(), found.end(), [&here](const shared_ptr<Ship> &a, const shared_ptr<Ship> &b) {
		if(a->IsDisabled() != b->IsDisabled())
			return a->IsDisabled();
		return a->Position().Distance(here) < b->Position().Distance(here);
	});
	allTargets.assign(found.begin(), found.end());
	Refilter();
}



shared_ptr<Ship> TargetPickerPanel::NearestDisabled(const PlayerInfo &player, const list<shared_ptr<Ship>> &ships)
{
	const Ship *flagship = player.Flagship();
	if(!flagship)
		return nullptr;
	ForgetHiddenIfMoved(*flagship);

	shared_ptr<Ship> nearest;
	double nearestDistance = 0.;
	for(const shared_ptr<Ship> &ship : ships)
		if(ship->IsDisabled() && IsCandidate(*ship, *flagship) && IsListed(*ship) && !IsHidden(*ship))
		{
			const double distance = ship->Position().Distance(flagship->Position());
			if(!nearest || distance < nearestDistance)
			{
				nearest = ship;
				nearestDistance = distance;
			}
		}
	return nearest;
}



void TargetPickerPanel::Step()
{
	// A long press on a ship targets and boards it.
	if(isPressed && dragDistance <= 15. && ++holdSteps == LONG_PRESS_STEPS)
	{
		const int index = CellAt(pressPoint);
		if(index >= 0 && CrossAt(pressPoint, index) < 0)
		{
			isPressed = false;
			Select(index, true);
		}
	}
}



void TargetPickerPanel::Draw()
{
	DrawBackdrop();
	FillShader::Fill(Point(), Point(Screen::Width(), Screen::Height()), Color(0.f, .7f));

	const Font &font = FontSet::Get(14);
	const Font &bigFont = FontSet::Get(18);
	const Color &dim = *GameData::Colors().Get("dim");
	const Color &medium = *GameData::Colors().Get("medium");
	const Color &bright = *GameData::Colors().Get("bright");
	const Color &background = *GameData::Colors().Get("panel background");

	const string title = targets.empty() ? "Nothing to target here."
		: "Select a target (" + to_string(targets.size()) + "). Hold to board.";
	bigFont.Draw(title, Point(Screen::Left() + 20., Screen::Top() + 15.), bright);

	// The toggles, top right.
	static const string TOGGLE_NAMES[TOGGLES] = {"My fleet", "Friendly", "Hidden"};
	const bool *const TOGGLE_VALUES[TOGGLES] = {&showFleet, &showFriendly, &showHidden};
	for(int which = 0; which < TOGGLES; ++which)
	{
		const bool on = *TOGGLE_VALUES[which];
		const Rectangle box = ToggleBox(which);
		FillShader::Fill(box.Center(), box.Dimensions(), on ? bright : dim);
		FillShader::Fill(box.Center(), box.Dimensions() - Point(4., 4.), background);
		const string label = TOGGLE_NAMES[which] + (on ? ": shown" : ": hidden");
		font.Draw(label, box.Center() - .5 * Point(font.Width(label), font.Height()), on ? bright : medium);
	}
	const Ship *flagship = player.Flagship();
	if(!flagship)
		return;
	const Ship *current = flagship->GetTargetShip().get();

	for(size_t i = 0; i < targets.size(); ++i)
	{
		const shared_ptr<Ship> ship = targets[i].lock();
		if(!ship)
			continue;
		const Point topLeft = CellTopLeft(i);
		if(topLeft.Y() > Screen::Bottom() || topLeft.Y() + CELL_HEIGHT < Screen::Top())
			continue;
		const Point center = topLeft + .5 * Point(CELL_WIDTH, CELL_HEIGHT);

		// The current target gets a bright frame.
		if(ship.get() == current)
			FillShader::Fill(center, Point(CELL_WIDTH + 4., CELL_HEIGHT + 4.), bright);
		FillShader::Fill(center, Point(CELL_WIDTH, CELL_HEIGHT), background);

		// The ship, scaled to fit its square.
		const Sprite *sprite = ship->GetSprite();
		if(sprite && sprite->Width() && sprite->Height())
		{
			const double scale = min(1., (ICON_SIZE - 10.) / max(sprite->Width(), sprite->Height()));
			SpriteShader::Draw(sprite, topLeft + Point(10. + .5 * ICON_SIZE, .5 * CELL_HEIGHT),
				scale, ship->GetSwizzle());
		}

		// Name, model, government, distance.
		const Point textLeft = topLeft + Point(ICON_SIZE + 15., 8.);
		const Layout layout(static_cast<int>(TEXT_WIDTH), Truncate::BACK);
		const double line = font.Height() + 3.;
		font.Draw({ship->GivenName(), layout}, textLeft, bright);
		font.Draw({ship->DisplayModelName(), layout}, textLeft + Point(0., line), medium);
		const string government = ship->IsDisabled() ? "disabled"
			: ship->GetGovernment() ? ship->GetGovernment()->DisplayName() : "No Government";
		font.Draw({government, layout}, textLeft + Point(0., 2. * line), RelationColor(*ship));
		const double distance = ship->Position().Distance(flagship->Position());
		font.Draw({"distance " + Format::Number(round(distance)), layout}, textLeft + Point(0., 3. * line), medium);

		// Shield and hull bars.
		const double barTop = textLeft.Y() + 4. * line + 2.;
		const auto bar = [&](double y, double fraction, const Color &color) {
			FillShader::Fill(Point(textLeft.X() + .5 * TEXT_WIDTH, y), Point(TEXT_WIDTH, 4.), dim);
			fraction = max(0., min(1., fraction));
			if(fraction > 0.)
				FillShader::Fill(Point(textLeft.X() + .5 * TEXT_WIDTH * fraction, y),
					Point(TEXT_WIDTH * fraction, 4.), color);
		};
		bar(barTop, ship->ShieldFraction(), Color(.4f, .6f, 1.f, 1.f));
		bar(barTop + 8., ship->HullFraction(), Color(1.f, .7f, .3f, 1.f));

		// Hidden ships (listed only with the Hidden toggle on) are dimmed.
		if(IsHidden(*ship))
			FillShader::Fill(center, Point(CELL_WIDTH, CELL_HEIGHT), Color(0.f, .55f));
		// The hide crosses: top left hides the model, top right this ship. Bright when in effect.
		const Point modelCross = topLeft + Point(.5 * CROSS_RANGE, .5 * CROSS_RANGE);
		const Point shipCross = topLeft + Point(CELL_WIDTH - .5 * CROSS_RANGE, .5 * CROSS_RANGE);
		DrawCross(modelCross, hiddenModels.contains(ship->TrueModelName()) ? bright : medium);
		font.Draw("all", modelCross + Point(-.5 * font.Width("all"), .5 * CROSS_SIZE + 2.), medium);
		DrawCross(shipCross, IsHiddenShip(ship.get()) ? bright : medium);
	}
}



bool TargetPickerPanel::KeyDown(SDL_Keycode key, Uint16 mod, const Command &command, bool isNewPress)
{
	if(key == SDLK_ESCAPE || key == SDLK_AC_BACK || command.Has(Command::MENU))
		GetUI().Pop(this);
	return true;
}



bool TargetPickerPanel::Click(int x, int y, MouseButton button, int clicks)
{
	isPressed = true;
	dragDistance = 0.;
	holdSteps = 0;
	pressPoint = Point(x, y);
	return true;
}



bool TargetPickerPanel::Drag(double dx, double dy)
{
	if(!isPressed)
		return true;
	dragDistance += fabs(dx) + fabs(dy);
	scroll -= dy;
	ClampScroll();
	return true;
}



bool TargetPickerPanel::Release(int x, int y, MouseButton button)
{
	if(!isPressed)
		return true;
	isPressed = false;
	if(dragDistance > 15.)
		return true;

	const Point point(x, y);
	for(int which = 0; which < TOGGLES; ++which)
		if(ToggleBox(which).Contains(point))
		{
			bool &show = which == 0 ? showFleet : which == 1 ? showFriendly : showHidden;
			show = !show;
			Refilter();
			return true;
		}
	const int index = CellAt(point);
	if(index >= 0 && CrossAt(point, index) >= 0)
	{
		ToggleHidden(index, CrossAt(point, index));
		return true;
	}
	if(index >= 0)
	{
		Select(index, false);
		return true;
	}
	// A tap outside every ship closes the picker.
	GetUI().Pop(this);
	return true;
}



bool TargetPickerPanel::Scroll(double dx, double dy)
{
	scroll -= dy * 50.;
	ClampScroll();
	return true;
}



int TargetPickerPanel::Columns() const
{
	return max(1, static_cast<int>((Screen::Width() - 2. * GAP) / (CELL_WIDTH + GAP)));
}



Point TargetPickerPanel::CellTopLeft(int index) const
{
	const int columns = Columns();
	const double gridWidth = columns * CELL_WIDTH + (columns - 1) * GAP;
	return Point(-.5 * gridWidth + (index % columns) * (CELL_WIDTH + GAP),
		Screen::Top() + TOP_MARGIN + (index / columns) * (CELL_HEIGHT + GAP) - scroll);
}



void TargetPickerPanel::ClampScroll()
{
	const int rows = (static_cast<int>(targets.size()) + Columns() - 1) / Columns();
	const double contentHeight = TOP_MARGIN + rows * (CELL_HEIGHT + GAP);
	scroll = max(0., min(scroll, contentHeight - Screen::Height()));
}



void TargetPickerPanel::Refilter()
{
	targets.clear();
	for(const weak_ptr<Ship> &it : allTargets)
	{
		const shared_ptr<Ship> ship = it.lock();
		if(!ship)
			continue;
		if(IsListed(*ship))
			targets.push_back(ship);
	}
	ClampScroll();
}



int TargetPickerPanel::CellAt(const Point &point) const
{
	for(size_t i = 0; i < targets.size(); ++i)
		if(Rectangle::FromCorner(CellTopLeft(i), Point(CELL_WIDTH, CELL_HEIGHT)).Contains(point))
			return i;
	return -1;
}



int TargetPickerPanel::CrossAt(const Point &point, int index) const
{
	const Point topLeft = CellTopLeft(index);
	if(Rectangle::FromCorner(topLeft, Point(CROSS_RANGE, CROSS_RANGE)).Contains(point))
		return 0;
	if(Rectangle::FromCorner(topLeft + Point(CELL_WIDTH - CROSS_RANGE, 0.), Point(CROSS_RANGE, CROSS_RANGE)).Contains(point))
		return 1;
	return -1;
}



void TargetPickerPanel::ToggleHidden(int index, int cross)
{
	const shared_ptr<Ship> ship = targets[index].lock();
	if(!ship)
		return;
	if(cross == 0)
	{
		if(!hiddenModels.erase(ship->TrueModelName()))
			hiddenModels.insert(ship->TrueModelName());
	}
	else if(IsHiddenShip(ship.get()))
		erase_if(hiddenShips, [&ship](const weak_ptr<Ship> &it) { return it.lock() == ship; });
	else
		hiddenShips.push_back(ship);
	Refilter();
}



Rectangle TargetPickerPanel::ToggleBox(int which) const
{
	const double right = Screen::Right() - 20. - (TOGGLES - 1 - which) * (TOGGLE_WIDTH + GAP);
	return Rectangle::FromCorner(Point(right - TOGGLE_WIDTH, Screen::Top() + 8.), Point(TOGGLE_WIDTH, TOGGLE_HEIGHT));
}



void TargetPickerPanel::Select(int index, bool board)
{
	const shared_ptr<Ship> ship = targets[index].lock();
	Ship *flagship = player.Flagship();
	if(ship && flagship && ship->IsTargetable())
	{
		flagship->SetTargetShip(ship);
		player.SelectEscort(IsFriendlyEscort(*ship) ? ship.get() : nullptr, false);
		UI::PlaySound(UI::UISound::TARGET);
		// Boarding goes to the current target; apply it once this panel is gone.
		if(board)
			Command::InjectOnce(Command::BOARD, true);
	}
	GetUI().Pop(this);
}
