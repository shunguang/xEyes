#ifndef __CFG_BASE_H__
#define __CFG_BASE_H__

#include "libUtil/util.h"
#include "CfgDefs.h"
namespace xeyes {
	class CFG_EXPORT CfgBase {
	public:
		CfgBase();
		CfgBase(const CfgBase &x) = default;
		CfgBase& operator = (const CfgBase &x) = default;
		virtual ~CfgBase() = default;

		virtual boost::property_tree::ptree toPropertyTree() = 0;
		virtual void fromPropertyTree(const boost::property_tree::ptree &pt) = 0;
		virtual std::string toString()=0;

		static std::string toString(const boost::property_tree::ptree &pt);
	};
	typedef std::shared_ptr<CfgBase>		CfgBasePtr;
}

#endif




