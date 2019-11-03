#include "CfgBase.h"

using namespace std;
using namespace xeyes;
CfgBase::CfgBase()
{
}

std::string CfgBase::toString(const boost::property_tree::ptree &pt)
{
	std::ostringstream oss;
	boost::property_tree::xml_parser::xml_writer_settings<std::string> settings(' ', 4);
	boost::property_tree::xml_parser::write_xml(oss, pt, settings);

	return oss.str();
}


