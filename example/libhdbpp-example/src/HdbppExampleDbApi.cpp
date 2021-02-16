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

#include "HdbppExampleDbApi.hpp"

using namespace std;

namespace hdbpp
{

//=============================================================================
//=============================================================================
HdbppExampleDbApi::HdbppExampleDbApi(const string &id, const vector<string> &configuration)
{
}

//=============================================================================
//=============================================================================
HdbppExampleDbApi::~HdbppExampleDbApi()
{
}

//=============================================================================
//=============================================================================
void HdbppExampleDbApi::insert_event(Tango::EventData *event_data, const HdbEventDataType &data_type)
{
}

//=============================================================================
//=============================================================================
void HdbppExampleDbApi::insert_events(vector<tuple<Tango::EventData *, HdbEventDataType>> events) 
{
    
}

//=============================================================================
//=============================================================================
void HdbppExampleDbApi::insert_param_event(
    Tango::AttrConfEventData *param_event, const HdbEventDataType & /* data_type */)
{
}

//=============================================================================
//=============================================================================
void HdbppExampleDbApi::add_attribute(const std::string &fqdn_attr_name, int type, int format, int write_type)
{
}

//=============================================================================
//=============================================================================
void HdbppExampleDbApi::update_ttl(const std::string &fqdn_attr_name, unsigned int ttl)
{
}

//=============================================================================
//=============================================================================
void HdbppExampleDbApi::insert_history_event(const std::string &fqdn_attr_name, unsigned char event)
{
}

//=============================================================================
//=============================================================================
bool HdbppExampleDbApi::supported(HdbppFeatures feature)
{
}

} // namespace hdbpp
