/* Copyright (C) 2014-2017
   Elettra - Sincrotrone Trieste S.C.p.A.
   Strada Statale 14 - km 163,5 in AREA Science Park
   34149 Basovizza, Trieste, Italy.

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

#include <dlfcn.h>
#include "HdbClient.h"

using namespace std;

namespace hdbpp
{
//=============================================================================
//=============================================================================
void string_vector2map(const vector<string> &config, const string &separator, map<string, string> &results)
{
    for (const auto &item : config)
    {
        auto found_eq = item.find_first_of(separator);

        if (found_eq != string::npos && found_eq > 0)
            results.insert(make_pair(item.substr(0, found_eq), item.substr(found_eq + 1)));
    }
}

//=============================================================================
//=============================================================================
auto HdbDBFactory::create_db(const string &id, const std::vector<std::string> &configuration) -> AbstractDB*
{
    return new HdbClient(id, configuration);
}

//=============================================================================
//=============================================================================
DBFactory *getDBFactory()
{
    auto *factory = new HdbDBFactory();
    return static_cast<DBFactory *>(factory);
}

HdbClient::HdbClient(const string &id, const std::vector<std::string> &configuration)
{
    map<string, string> db_conf;
    string_vector2map(configuration, "=", db_conf);
    string libname;

    try
    {
        libname = db_conf.at("libname");
    }
    catch (const std::out_of_range &e)
    {
        cout << __func__ << ": "
             << "Configuration parsing error looking for key 'libname'" << endl;
        exit(1);
    }

    if ((hLib = dlopen(libname.c_str(), RTLD_NOW /*|RTLD_GLOBAL*/)) != nullptr)
    {
        // cant find a good way to do this within guidelines, so for now we add
        // this special ignore case
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-cstyle-cast)
        if (getDBFactory_t *create_factory = (getDBFactory_t *)dlsym(hLib, "getDBFactory"))
        {
            db_factory = create_factory();
            db = db_factory->create_db(id, configuration);

            if (db == nullptr)
            {
                cout << __func__ << ": Error creating db (library: " << libname << ")" << endl;
                exit(1);
            }
        }
        else
        {
            cout << __func__ << ": Error loading symbol getDBFactory from library: " << libname << endl;
            exit(1);
        }
    }
    else
    {
        db = nullptr;
        cout << __func__ << ": Error loading library: " << libname << ". Error report: " << dlerror() << endl;
        exit(1);
    }
}

//=============================================================================
//=============================================================================
HdbClient::~HdbClient()
{
    delete db;
    delete db_factory;
    dlclose(hLib);
}

//=============================================================================
//=============================================================================
void HdbClient::insert_event(Tango::EventData *event, const HdbEventDataType &data_type)
{
    db->insert_event(event, data_type);
}

//=============================================================================
//=============================================================================
void HdbClient::insert_events(vector<tuple<Tango::EventData *, HdbEventDataType>> events)
{
    db->insert_events(events);
}

//=============================================================================
//=============================================================================
void HdbClient::insert_param_event(Tango::AttrConfEventData *data, const HdbEventDataType &data_type)
{
    db->insert_param_event(data, data_type);
}

//=============================================================================
//=============================================================================
void HdbClient::add_attribute(const string &name, int type, int format, int write_type)
{
    db->add_attribute(name, type, format, write_type);
}

//=============================================================================
//=============================================================================
void HdbClient::update_ttl(const string &name, unsigned int ttl)
{
    db->update_ttl(name, ttl);
}

//=============================================================================
//=============================================================================
void HdbClient::insert_history_event(const string &name, unsigned char event)
{
    db->insert_history_event(name, event);
}

//=============================================================================
//=============================================================================
bool HdbClient::supported(HdbppFeatures feature)
{
    return db->supported(feature);
}

} // namespace hdbpp
