/* TargetPickerPanel.h
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

#pragma once

#include "Panel.h"

#include "Point.h"
#include "Rectangle.h"

#include <list>
#include <memory>
#include <vector>

class PlayerInfo;
class Ship;



// t7: a grid of every ship the flagship can target in this system, opened by a long
// press on the target display. Tapping a ship targets it; a long press also boards it.
class TargetPickerPanel : public Panel {
public:
	TargetPickerPanel(PlayerInfo &player, const std::list<std::shared_ptr<Ship>> &ships);

	// t7: the nearest disabled ship the picker would list now (its toggles and hidden ships
	// applied), for the boarding light on the target display. Null if there is none.
	static std::shared_ptr<Ship> NearestDisabled(const PlayerInfo &player,
		const std::list<std::shared_ptr<Ship>> &ships);
	// t7: hide a ship, or every ship of its model, as the picker's crosses do.
	static void Hide(const PlayerInfo &player, const std::shared_ptr<Ship> &ship, bool allOfModel);

	virtual void Step() override;
	virtual void Draw() override;


protected:
	virtual bool KeyDown(SDL_Keycode key, Uint16 mod, const Command &command, bool isNewPress) override;
	virtual bool Click(int x, int y, MouseButton button, int clicks) override;
	virtual bool Drag(double dx, double dy) override;
	virtual bool Release(int x, int y, MouseButton button) override;
	virtual bool Scroll(double dx, double dy) override;


private:
	int Columns() const;
	Point CellTopLeft(int index) const;
	void ClampScroll();
	void Refilter();
	int CellAt(const Point &point) const;
	Rectangle ToggleBox(int which) const;
	// t7: which hide cross of a cell is at this point: 0 for the model, 1 for the ship, -1 for none.
	int CrossAt(const Point &point, int index) const;
	void ToggleHidden(int index, int cross);
	void Select(int index, bool board);


private:
	PlayerInfo &player;
	// Every candidate, and the ones the toggles let through.
	std::vector<std::weak_ptr<Ship>> allTargets;
	std::vector<std::weak_ptr<Ship>> targets;

	double scroll = 0.;
	double dragDistance = 0.;
	bool isPressed = false;
	Point pressPoint;
	int holdSteps = 0;
};
