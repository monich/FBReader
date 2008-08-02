/*
 * Copyright (C) 2004-2008 Geometer Plus <contact@geometerplus.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#ifndef __ZLTOOLBAR_H__
#define __ZLTOOLBAR_H__

#include <string>
#include <vector>
#include <map>
#include <set>

#include <shared_ptr.h>

class ZLResource;
class ZLPopupData;

class ZLToolbar {

public:
	class Item {

	public:
		enum Type {
			PLAIN_BUTTON,
			MENU_BUTTON,
			TOGGLE_BUTTON,
			TEXT_FIELD,
			SEPARATOR
		};

	public:
		Item();
		virtual ~Item();

		virtual Type type() const = 0;
	};

	class ActionItem : public Item {

	public:
		ActionItem(const std::string &actionId, const ZLResource &tooltip);
		const std::string &actionId() const;
		const std::string &tooltip() const;

	private:
		const std::string myActionId;

	protected:
		const ZLResource &myTooltip;
	};

	class AbstractButtonItem : public ActionItem {

	public:
		AbstractButtonItem(const std::string &actionId, const ZLResource &tooltip);
		const std::string &iconName() const;
	};

	class PlainButtonItem : public AbstractButtonItem {

	public:
		PlainButtonItem(const std::string &actionId, const ZLResource &tooltip);

		Type type() const;
	};

	class MenuButtonItem : public AbstractButtonItem {

	public:
		MenuButtonItem(const ZLToolbar &toolbar, const std::string &actionId, const ZLResource &tooltip);

		const std::string &popupTooltip() const;
		shared_ptr<ZLPopupData> popupData() const;
	
	private:
		Type type() const;

	private:
		const ZLToolbar &myToolbar;
	};

	class ButtonGroup;

	class ToggleButtonItem : public AbstractButtonItem {

	public:
		ToggleButtonItem(const std::string &actionId, ButtonGroup &group, const ZLResource &tooltip);

		Type type() const;

		ButtonGroup &buttonGroup();
		void press();
		bool isPressed() const;

	private:
		ButtonGroup &myGroup;
	};

	class ButtonGroup {

	private:
		ButtonGroup();
		void press(const ToggleButtonItem *item);

	private:
		typedef std::set<const ToggleButtonItem*> ItemSet;

		ItemSet Items;
		const ToggleButtonItem *PressedItem;

	friend class ToggleButtonItem;
	friend class ZLToolbar;
	friend class ZLApplicationWindow;
	};

	class SeparatorItem : public Item {

	public:
		Type type() const;
	};

	class TextFieldItem : public ActionItem {

	public:
		TextFieldItem(const std::string &actionId, const std::string &parameterId, int maxWidth, const ZLResource &tooltip);
		Type type() const;
		int maxWidth() const;
		const std::string &parameterId() const;

	private:
		const std::string myParameterId;
		const int myMaxWidth;
	};

public:
	typedef shared_ptr<Item> ItemPtr;
	typedef std::vector<ItemPtr> ItemVector;

	ZLToolbar();
	void addPlainButton(const std::string &actionId);
	void addMenuButton(const std::string &actionId);
	ToggleButtonItem &addToggleButton(const std::string &actionId, const std::string &groupId);
	void addTextField(const std::string &actionId, const std::string &parameterId, int maxWidth);
	void addSeparator();

	const ItemVector &items() const;

	void registerPopupData(const std::string &actionId, shared_ptr<ZLPopupData> popupData);

private:
	ButtonGroup &getButtonGroup(const std::string &id);

private:
	ItemVector myItems;
	const ZLResource &myResource;
	std::map<std::string, shared_ptr<ZLPopupData> > myPopupDataMap;
	std::map<std::string, shared_ptr<ButtonGroup> > myButtonGroups;

friend class ZLApplication;
};

#endif /* __ZLTOOLBAR_H__ */
