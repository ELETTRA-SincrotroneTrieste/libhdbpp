/* Copyright (C) : 2014-2019
   European Synchrotron Radiation Facility
   BP 220, Grenoble 38043, FRANCE

   This file is part of libhdb++.

   libhdb++ is free software: you can redistribute it and/or modify
   it under the terms of the Lesser GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   libhdb++ is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the Lesser
   GNU General Public License for more details.

   You should have received a copy of the Lesser GNU General Public License
   along with libhdb++.  If not, see <http://www.gnu.org/licenses/>. */

#include "HdbppExampleDb.hpp"

#include "HdbppExampleDbApi.hpp"

namespace hdbpp
{
//=============================================================================
//=============================================================================
AbstractDB *HdbppExampleDbFactory::create_db(const string &id, const vector<string> &configuration)
{
    return new hdbpp::HdbppExampleDbApi(id, configuration);
}
} // namespace hdbpp

//=============================================================================
//=============================================================================
hdbpp::DBFactory *getDBFactory()
{
    auto *factory = new hdbpp::HdbppExampleDbFactory();
    return static_cast<hdbpp::DBFactory *>(factory);
}
