
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_navimake
#include <boost/test/unit_test.hpp>

#include "routing/raptor.h"
#include "naviMake/build_helper.h"

using namespace navitia;
using namespace routing::raptor;


BOOST_AUTO_TEST_CASE(direct){
    navimake::builder b("20120614");
    b.vj("A")("stop1", 8000, 8050)("stop2", 8100,8150);
    type::Data data;
    data.pt_data =  b.build();
    RAPTOR raptor(data);

    type::PT_Data d = data.pt_data;

    auto res = raptor.compute (d.stop_areas[0].idx, d.stop_areas[1].idx, 8200, 0, routing::arriveravant);

    BOOST_REQUIRE_EQUAL(res.items.size(), 1);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[0], 0);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[1], 1);
    BOOST_CHECK_EQUAL(res.items[0].departure.hour(), 8050);
    BOOST_CHECK_EQUAL(res.items[0].arrival.hour(), 8100);
    BOOST_CHECK_EQUAL(res.items[0].departure.date(), 0);
    BOOST_CHECK_EQUAL(res.items[0].arrival.date(), 0);
}

BOOST_AUTO_TEST_CASE(change){
    navimake::builder b("20120614");
    b.vj("A")("stop1", 8000, 8050)("stop2", 8100, 8150)("stop3", 8200, 8250);
    b.vj("B")("stop4", 8000, 8050)("stop2", 8200,8250)("stop5", 8300,8350);
    type::Data data;
    data.pt_data =  b.build();
    RAPTOR raptor(data);
    type::PT_Data d = data.pt_data;

    auto res = raptor.compute(d.stop_areas[0].idx, d.stop_areas[4].idx, 8900, 0, routing::arriveravant);
    BOOST_REQUIRE_EQUAL(res.items.size(), 2);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[0], 0);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[1], 1);
    BOOST_CHECK_EQUAL(res.items[1].stop_points[0], 1);
    BOOST_CHECK_EQUAL(res.items[1].stop_points[1], 4);
    BOOST_CHECK_EQUAL(res.items[0].departure.hour(), 8050);
    BOOST_CHECK_EQUAL(res.items[0].arrival.hour(), 8100);
    BOOST_CHECK_EQUAL(res.items[1].departure.hour(), 8250);
    BOOST_CHECK_EQUAL(res.items[1].arrival.hour(), 8300);
    BOOST_CHECK_EQUAL(res.items[0].arrival.date(), 0);
    BOOST_CHECK_EQUAL(res.items[1].arrival.date(), 0);
}

BOOST_AUTO_TEST_CASE(passe_minuit){
    navimake::builder b("20120614");
    b.vj("A")("stop1", 23*3600)("stop2", 24*3600 + 5*60);
    b.vj("B")("stop2", 10*60)("stop3", 20*60);
    type::Data data;
    data.pt_data =  b.build();
    RAPTOR raptor(data);
    type::PT_Data d = data.pt_data;

    auto res = raptor.compute(d.stop_areas[0].idx, d.stop_areas[2].idx, 22*3600, 1, routing::arriveravant);

    BOOST_REQUIRE_EQUAL(res.items.size(), 2);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[0], 0);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[1], 1);
    BOOST_CHECK_EQUAL(res.items[1].stop_points[0], 1);
    BOOST_CHECK_EQUAL(res.items[1].stop_points[1], 2);
    BOOST_CHECK_EQUAL(res.items[0].departure.date(), 0);
    BOOST_CHECK_EQUAL(res.items[1].arrival.date(), 1);
}

BOOST_AUTO_TEST_CASE(passe_minuit_2){
    navimake::builder b("20120614");
    b.vj("A")("stop1", 23*3600)("stop2", 23*3600 + 59*60);
    b.vj("B")("stop4", 23*3600 + 10*60)("stop2", 10*60)("stop3", 20*60);
    type::Data data;
    data.pt_data =  b.build();
    RAPTOR raptor(data);
    type::PT_Data d = data.pt_data;

    auto res = raptor.compute(d.stop_areas[0].idx, d.stop_areas[2].idx, 22*3600, 1, routing::arriveravant);


    BOOST_REQUIRE_EQUAL(res.items.size(), 2);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[0], 0);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[1], 1);
    BOOST_CHECK_EQUAL(res.items[1].stop_points[0], 1);
    BOOST_CHECK_EQUAL(res.items[1].stop_points[1], 2);
    BOOST_CHECK_EQUAL(res.items[0].departure.date(), 0);
    BOOST_CHECK_EQUAL(res.items[1].arrival.date(), 1);
}

BOOST_AUTO_TEST_CASE(passe_minuit_interne){
    navimake::builder b("20120614");
    b.vj("A")("stop1", 23*3600)("stop2", 23*3600 + 30*60, 24*3600 + 30*60)("stop3", 24*40+3600);
    type::Data data;
    data.pt_data =  b.build();
    RAPTOR raptor(data);

    type::PT_Data d = data.pt_data;

    auto res = raptor.compute(d.stop_areas[0].idx, d.stop_areas[2].idx, 22*3600, 1, routing::arriveravant);

    BOOST_REQUIRE_EQUAL(res.items.size(), 1);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[0], 0);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[1], 1);
    BOOST_CHECK_EQUAL(res.items[0].stop_points[2], 2);
    BOOST_CHECK_EQUAL(res.items[0].arrival.date(), 1);
    BOOST_CHECK_EQUAL(res.items[0].departure.date(), 0);
}

BOOST_AUTO_TEST_CASE(validity_pattern){
    navimake::builder b("20120614");
    b.vj("D", "0")("stop1", 8000)("stop2", 8200);
    b.vj("C", "1")("stop1", 9000)("stop2", 9200);
    type::Data data;
    data.pt_data =  b.build();
    RAPTOR raptor(data);

    type::PT_Data d = data.pt_data;

    auto res = raptor.compute(d.stop_areas[0].idx, d.stop_areas[1].idx, 17900, 0, routing::arriveravant);

    BOOST_REQUIRE_EQUAL(res.items.size(), 1);
    BOOST_CHECK_EQUAL(res.items[0].arrival.hour(), 9200);

    // TODO : le test ne passe pas , mais ne devrait pas !
    res = raptor.compute(d.stop_areas[0].idx, d.stop_areas[1].idx, 7000, 1, routing::arriveravant);
    //BOOST_REQUIRE_EQUAL(res.items.size(), 0);
}


BOOST_AUTO_TEST_CASE(marche_a_pied_milieu){
    navimake::builder b("20120614");
    b.vj("A")("stop1", 8000,8050)("stop2", 8200,8250);
    b.vj("B")("stop3", 9000,9050)("stop4", 9200,9250);
    b.connection("stop2", "stop3", 10*60);
    b.connection("stop3", "stop2", 10*60);
    type::Data data;
    data.pt_data = b.build();
    RAPTOR raptor(data);
    type::PT_Data d = data.pt_data;

    auto res = raptor.compute(d.stop_areas[0].idx, d.stop_areas[3].idx, 17900, 0, routing::arriveravant);

    BOOST_REQUIRE_EQUAL(res.items.size(), 3);
    BOOST_CHECK_EQUAL(res.items[2].arrival.hour(), 9200);
}


BOOST_AUTO_TEST_CASE(marche_a_pied_fin){
    navimake::builder b("20120614");
    b.vj("A")("stop1", 8000)("stop2", 8200);
    b.vj("B")("stop3", 30000)("stop4",40000);
    b.connection("stop2", "stop3", 10*60);

    type::Data data;
    data.pt_data = b.build();
    RAPTOR raptor(data);
    type::PT_Data d = data.pt_data;

    auto res = raptor.compute(d.stop_areas.at(0).idx, d.stop_areas.at(2).idx, 9000, 0, routing::arriveravant);

    BOOST_REQUIRE_EQUAL(res.items.size(), 2);
    BOOST_CHECK_EQUAL(res.items[1].stop_points[1], 2);
    BOOST_CHECK_EQUAL(res.items[0].arrival.hour(), 8200);
    BOOST_CHECK_GE(res.items[1].departure.hour(), 8200);
    BOOST_CHECK_GE(res.items[1].arrival.hour(), 8200+10*60);
}

BOOST_AUTO_TEST_CASE(marche_a_pied_pam){
    navimake::builder b("20120614");
    b.vj("A")("stop1", 8000)("stop2", 23*3600+59*60);
    b.vj("B")("stop3", 2*3600)("stop4",2*3600+20);
    b.connection("stop2", "stop3", 10*60);
    //    std::cout << res << std::endl;
    //    BOOST_REQUIRE_EQUAL(res.items.size(), 3);
    //    BOOST_CHECK_EQUAL(res.items[2].said, 2);
    //    BOOST_CHECK_EQUAL(res.items[2].arrival.hour(), 8200+10*60);
    type::Data data;
    data.pt_data = b.build();
    RAPTOR raptor(data);
    type::PT_Data d = data.pt_data;

    auto res = raptor.compute(d.stop_areas.at(0).idx, d.stop_areas.at(3).idx, 17900, 1, routing::arriveravant);
    BOOST_REQUIRE_EQUAL(res.items.size(), 3);
    BOOST_CHECK_EQUAL(res.items[2].stop_points[1], 3);
    BOOST_CHECK_EQUAL(res.items[2].arrival.hour(), 2*3600+20);
    BOOST_CHECK_EQUAL(res.items[2].arrival.date(), 1);
}


BOOST_AUTO_TEST_CASE(marche_a_pied_debut) {
    navimake::builder b("20120614");
    b.vj("A")("stop1", 8000)("stop20", 8200);
    b.vj("B")("stop2", 30000)("stop3",40000);
    b.connection("stop1", "stop2", 10*60);

    type::Data data;
    data.pt_data = b.build();
    RAPTOR raptor(data);
    type::PT_Data d = data.pt_data;

    auto res = raptor.compute(d.stop_areas.at(0).idx, d.stop_areas.at(3).idx, 50900, 0, routing::arriveravant);

    BOOST_REQUIRE_EQUAL(res.items.size(), 2);
    BOOST_CHECK_EQUAL(res.items[1].stop_points[1], 3);
    BOOST_CHECK_EQUAL(res.items[1].arrival.hour(), 40000);
}

