#include "gtfs_parser.h"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <fstream>
#include "boost/tuple/tuple_io.hpp"

template<class T1, class T2>
struct True{
    bool operator()(const T1 &, const T2 &) {
        return true;
    }
};

int main(int, char **) {
/*    GtfsParser p("/home/kinou/workspace/gtfs", "20101101");
    Data data = p.getData();
    data.build_index();
    
   

    
    BOOST_FOREACH(auto sp, data.stoppoint_of_stoparea.get(42)){
       std::cout << sp->name << " " << sp->code << " " << sp->stop_area_idx << std::endl;
    }

    std::string foo("1");
    data.find(&StopArea::code, "1");
    data.find(&StopPoint::stop_area_idx, 1);
    BOOST_FOREACH(auto sp, data.find(&StopPoint::name, "Le Petit Bois")){
       std::cout << sp->name << " " << sp->code << " " << sp->stop_area_idx << std::endl;
    }

    {
        std::ofstream ofs("data.nav");
        boost::archive::binary_oarchive oa(ofs);
        oa << data;
        Index<decltype(data.stop_times)> idx(data.stop_times.begin(), data.stop_times.end());
        oa << idx;
    }
  */  std::ifstream ifs("data.nav");
    boost::archive::binary_iarchive ia(ifs);
    Data data2;
    Index<decltype(data2.stop_times)> idx2(data2.stop_times.begin(), data2.stop_times.end());
    ia >> data2;// >> idx2;



    BOOST_FOREACH(auto sp, data2.stoppoint_of_stoparea.get(42)){
        std::cout << sp->name << " " << sp->code << " " << sp->stop_area_idx << std::endl;
    }

    data2.find(&StopArea::code, "1");
    data2.find(&StopPoint::stop_area_idx, 1);
    BOOST_FOREACH(auto sp, data2.find(&StopPoint::name, "Le Petit Bois")){
        std::cout << sp->name << " " << sp->code << " " << sp->stop_area_idx << std::endl;
    }

    auto sa = data2.stop_area_by_name.get(0);
    auto sa2 = data2.stop_area_by_name.get(data2.stop_areas.size() -1);
    std::cout << sa.name << " " << sa2.name << " " << sa2.code << std::endl;

    BOOST_FOREACH(auto line, data2.lines){
        std::cout << line.name << std::endl;
    }

    BOOST_FOREACH(auto line, data2.lines.filter(&Line::name, "Noctambus 2B")) {
        std::cout << line.name << std::endl;
    }

    BOOST_FOREACH(auto line, data2.lines.filter_match(&Line::name, ".*olaire.*").filter(&Line::code, "S14")) {
        std::cout << line.name << " " << line.code << std::endl;
    }
    auto idx = make_index(data2.lines.filter_match(&Line::name, ".*olaire.*"));
    auto subset = data2.lines.filter_match(&Line::name, ".*olaire.*");
    //    typedef Subset<boost::filter_iterator<matches<Line>, Container<Line>::iter<Line> > Moo;
    //Index<decltype(subset)> idx(subset.begin(), subset.end());
    std::cout << "Test de l'index" << std::endl;
    //  auto line = *(idx.begin());
    BOOST_FOREACH(auto line, std::make_pair(idx.begin(), idx.end())) {
        std::cout << line.name << " " << line.code << std::endl;
    }
    std::cout << "indes trié" << std::endl;
    auto idx3 = make_index(data2.lines.order());
    BOOST_FOREACH(auto line3, std::make_pair(idx3.begin(), idx3.end())) {
        std::cout << line3.name << " " << line3.code << std::endl;
    }
    

    std::cout << "join" << std::endl;
    auto ton = join_iterator<decltype(data2.lines.items), decltype(data2.lines.items), True<Line, Line> >(data2.lines.items, data2.lines.items, True<Line, Line>());
    std::cout << boost::get<0>(*ton)->name << " - " << boost::get<1>(*ton)->name << std::endl;
    for (int i = 0; i < 50; i++) {
        ton++;
        std::cout << boost::get<0>(*ton)->name << " - " << boost::get<1>(*ton)->name << std::endl;
        
    }
    /*BOOST_FOREACH(auto mou, ton){
        std::cout << get<0>(mou).name << " - " << get<1>(mou).name << std::endl;
    }*/

}
