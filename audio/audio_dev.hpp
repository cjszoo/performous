#ifndef AUDIO_DEV_HPP_INCLUDED
#define AUDIO_DEV_HPP_INCLUDED

#include "audio.hpp"
#include "plugin.hpp"

namespace da {
	struct record::dev {
		virtual ~dev() {}
	};
	typedef boost::plugin::registry<record::dev, settings&, devinfo> record_plugin;
}

#endif