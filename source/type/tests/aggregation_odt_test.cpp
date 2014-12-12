/* Copyright © 2001-2014, Canal TP and/or its affiliates. All rights reserved.

This file is part of Navitia,
    the software to build cool stuff with public transport.

Hope you'll enjoy and contribute to this project,
    powered by Canal TP (www.canaltp.fr).
Help us simplify mobility and open public transport:
    a non ending quest to the responsive locomotion way of traveling!

LICENCE: This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.

Stay tuned using
twitter @navitia
IRC #navitia on freenode
https://groups.google.com/d/forum/navitia
www.navitia.io
*/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_navimake
#include <boost/test/unit_test.hpp>

#include "type/type.h"
#include "ed/build_helper.h"
#include "type/pt_data.h"

struct aggregate_odt_fixture {
    ed::builder b;
    navitia::type::JourneyPattern* jp ;
    navitia::type::VehicleJourney* vj;
    aggregate_odt_fixture() : b("20140210"){
        jp = new navitia::type::JourneyPattern();
        vj = new navitia::type::VehicleJourney();
        vj->vehicle_journey_type = navitia::type::VehicleJourneyType::regular;
        b.data->pt_data->vehicle_journeys_map["regular"] = vj;
        b.data->pt_data->vehicle_journeys.push_back(vj);

        vj = new navitia::type::VehicleJourney();
        vj->vehicle_journey_type = navitia::type::VehicleJourneyType::virtual_with_stop_time;
        b.data->pt_data->vehicle_journeys_map["virtual_with_stop_time"] = vj;
        b.data->pt_data->vehicle_journeys.push_back(vj);

        vj = new navitia::type::VehicleJourney();
        vj->vehicle_journey_type = navitia::type::VehicleJourneyType::virtual_without_stop_time;
        b.data->pt_data->vehicle_journeys_map["virtual_without_stop_time"] = vj;
        b.data->pt_data->vehicle_journeys.push_back(vj);

        vj = new navitia::type::VehicleJourney();
        vj->vehicle_journey_type = navitia::type::VehicleJourneyType::stop_point_to_stop_point;
        b.data->pt_data->vehicle_journeys_map["stop_point_to_stop_point"] = vj;
        b.data->pt_data->vehicle_journeys.push_back(vj);

        vj = new navitia::type::VehicleJourney();
        vj->vehicle_journey_type = navitia::type::VehicleJourneyType::adress_to_stop_point;
        b.data->pt_data->vehicle_journeys_map["address_to_stop_point"] = vj;
        b.data->pt_data->vehicle_journeys.push_back(vj);

        vj = new navitia::type::VehicleJourney();
        vj->vehicle_journey_type = navitia::type::VehicleJourneyType::odt_point_to_point;
        b.data->pt_data->vehicle_journeys_map["odt_point_to_point"] = vj;
        b.data->pt_data->vehicle_journeys.push_back(vj);
    }


};

BOOST_FIXTURE_TEST_CASE(vj_none_odt_test, aggregate_odt_fixture) {
    vj = b.data->pt_data->vehicle_journeys_map["regular"];
    BOOST_CHECK_EQUAL(vj->is_none_odt(), true);
    BOOST_CHECK_EQUAL(vj->is_virtual_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_zonal_odt(), false);
}

BOOST_FIXTURE_TEST_CASE(vj_virtual_odt_test, aggregate_odt_fixture) {
    vj = b.data->pt_data->vehicle_journeys_map["virtual_with_stop_time"];
    BOOST_CHECK_EQUAL(vj->is_none_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_virtual_odt(), true);
    BOOST_CHECK_EQUAL(vj->is_zonal_odt(), false);
}

BOOST_FIXTURE_TEST_CASE(vj_zonal_odt_test1, aggregate_odt_fixture) {
    vj = b.data->pt_data->vehicle_journeys_map["virtual_without_stop_time"];
    BOOST_CHECK_EQUAL(vj->is_none_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_virtual_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_zonal_odt(), true);
}

BOOST_FIXTURE_TEST_CASE(vj_zonal_odt_test2, aggregate_odt_fixture) {
    vj = b.data->pt_data->vehicle_journeys_map["stop_point_to_stop_point"];
    BOOST_CHECK_EQUAL(vj->is_none_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_virtual_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_zonal_odt(), true);
}

BOOST_FIXTURE_TEST_CASE(vj_zonal_odt_test3, aggregate_odt_fixture) {
    vj = b.data->pt_data->vehicle_journeys_map["address_to_stop_point"];
    BOOST_CHECK_EQUAL(vj->is_none_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_virtual_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_zonal_odt(), true);
}

BOOST_FIXTURE_TEST_CASE(vj_zonal_odt_test4, aggregate_odt_fixture) {
    vj = b.data->pt_data->vehicle_journeys_map["odt_point_to_point"];
    BOOST_CHECK_EQUAL(vj->is_none_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_virtual_odt(), false);
    BOOST_CHECK_EQUAL(vj->is_zonal_odt(), true);
}


BOOST_FIXTURE_TEST_CASE(jp_none_odt_test, aggregate_odt_fixture) {
    vj = b.data->pt_data->vehicle_journeys_map["regular"];
    jp->vehicle_journey_list.push_back(vj);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), true);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), false);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), false);
}

BOOST_FIXTURE_TEST_CASE(jp_virtual_odt_test, aggregate_odt_fixture) {
    jp->vehicle_journey_list.clear();
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["regular"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_with_stop_time"]);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), false);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), true);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), false);
}

BOOST_FIXTURE_TEST_CASE(jp_virtual_and_zonal_odt_test1, aggregate_odt_fixture) {
    jp->vehicle_journey_list.clear();
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["regular"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_with_stop_time"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_without_stop_time"]);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), false);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), true);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), true);
}

BOOST_FIXTURE_TEST_CASE(jp_virtual_and_zonal_odt_test2, aggregate_odt_fixture) {
    jp->vehicle_journey_list.clear();
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["regular"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_with_stop_time"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_without_stop_time"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["stop_point_to_stop_point"]);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), false);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), true);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), true);
}

BOOST_FIXTURE_TEST_CASE(jp_virtual_and_zonal_odt_test3, aggregate_odt_fixture) {
    jp->vehicle_journey_list.clear();
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["regular"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_with_stop_time"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_without_stop_time"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["stop_point_to_stop_point"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["address_to_stop_point"]);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), false);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), true);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), true);
}

BOOST_FIXTURE_TEST_CASE(jp_virtual_and_zonal_odt_test4, aggregate_odt_fixture) {
    jp->vehicle_journey_list.clear();
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["regular"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_with_stop_time"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_without_stop_time"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["stop_point_to_stop_point"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["address_to_stop_point"]);
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["odt_point_to_point"]);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), false);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), true);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), true);
}


BOOST_FIXTURE_TEST_CASE(jp_zonal_odt_test1, aggregate_odt_fixture) {
    jp->vehicle_journey_list.clear();
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["virtual_without_stop_time"]);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), false);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), false);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), true);
}

BOOST_FIXTURE_TEST_CASE(jp_zonal_odt_test2, aggregate_odt_fixture) {
    jp->vehicle_journey_list.clear();
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["stop_point_to_stop_point"]);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), false);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), false);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), true);
}

BOOST_FIXTURE_TEST_CASE(jp_zonal_odt_test3, aggregate_odt_fixture) {
    jp->vehicle_journey_list.clear();
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["address_to_stop_point"]);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), false);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), false);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), true);
}

BOOST_FIXTURE_TEST_CASE(jp_zonal_odt_test4, aggregate_odt_fixture) {
    jp->vehicle_journey_list.clear();
    jp->vehicle_journey_list.push_back(b.data->pt_data->vehicle_journeys_map["odt_point_to_point"]);
    jp->build_odt_properties();

    BOOST_CHECK_EQUAL(jp->none_odt(), false);
    BOOST_CHECK_EQUAL(jp->virtual_odt(), false);
    BOOST_CHECK_EQUAL(jp->zonal_odt(), true);
}
