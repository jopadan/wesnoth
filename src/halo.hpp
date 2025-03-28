/*
	Copyright (C) 2003 - 2025
	by David White <dave@whitevine.net>
	Part of the Battle for Wesnoth Project https://www.wesnoth.org/

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.
	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY.

	See the COPYING file for more details.
*/

#pragma once

class display;
struct rect;

#include "map/location.hpp"

#include <memory>

namespace halo
{


class halo_impl;

class halo_record;

typedef std::shared_ptr<halo_record> handle;

enum ORIENTATION { NORMAL, HREVERSE, VREVERSE, HVREVERSE };

const int NO_HALO = 0;

class manager
{
public:
	manager();

	/**
	 * Add a haloing effect using 'image centered on (x,y).
	 * @return 	The handle to the halo object.
	 * @retval 	0 is the invalid handle.
	 *
	 * If the halo is attached to an item, it needs to be hidden if the
	 * shroud is active.  (Note it will be shown with the fog active.)
	 * If it is not attached to an item, the location should be set to -1, -1
	 */
	handle add(int x, int y, const std::string& image, const map_location& loc,
			halo::ORIENTATION orientation=NORMAL, bool infinite=true);

	/** Set the position of an existing haloing effect, according to its handle. */
	void set_location(const handle & h, int x, int y);

	/** Remove the halo with the given handle. */
	void remove(const handle & h);

	/** Process animations, remove deleted halos, and invalidate screen
	  * regions now requiring redraw. */
	void update();

	/** Render halos in region. */
	void render(const rect& r);

private:
	std::shared_ptr<halo_impl> impl_;
};

/**
 * RAII object which manages a halo. When it goes out of scope it removes the corresponding halo entry.
 */
class halo_record
{
public:
	halo_record(const halo_record&) = delete;
	halo_record& operator=(const halo_record&) = delete;

	halo_record();
	halo_record(int id, const std::shared_ptr<halo_impl> & my_manager);
	~halo_record();

	bool valid() const {
		return id_ != NO_HALO && !my_manager_.expired();
	}

	friend class manager;
private:
	int id_;
	std::weak_ptr<halo_impl> my_manager_;

};

} // end namespace halo
