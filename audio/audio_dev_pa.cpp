#include "audio_dev.hpp"
#include <portaudio.h>
#include <ostream>
#include <sstream>

namespace {
	using namespace da;
	class pa19_record: public record::dev {
		static int c_callback(const void* input, void*, unsigned long frames, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* userdata)
		{
			pa19_record& self = *static_cast<pa19_record*>(userdata);
			int16_t const* iptr = static_cast<int16_t const*>(input);
			try {
				std::vector<sample_t> buf(frames * self.s.channels());
				std::transform(iptr, iptr + buf.size(), buf.begin(), conv_from_s16);
				pcm_data data(&buf[0], frames, self.s.channels());
				self.s.callback()(data, self.s);
			} catch (std::exception& e) {
				self.s.debug(std::string("Exception from recording callback: ") + e.what());
			}
			return paContinue;
		}
		settings s;
		struct init {
			init() {
				PaError err = Pa_Initialize();
				if( err != paNoError ) throw std::runtime_error(std::string("Cannot initialize PortAudio: ") + Pa_GetErrorText(err));
			}
			~init() { Pa_Terminate(); }
		} initialize;
		struct strm {
			PaStream* handle;
			strm( pa19_record* rec) {
				PaStreamParameters p;
				if (rec->s.subdev().empty()) {
					p.device = Pa_GetDefaultInputDevice();
				} else {
					std::istringstream iss(rec->s.subdev());
					iss >> p.device;
					if (!iss.eof() || p.device < 0 || p.device > Pa_GetDeviceCount() - 1) throw std::invalid_argument("Invalid PortAudio device number");
				}
				p.channelCount = rec->s.channels();
				p.sampleFormat = paInt16;
				if (rec->s.frames() != settings::high) p.suggestedLatency = Pa_GetDeviceInfo(p.device)->defaultLowInputLatency;
				else p.suggestedLatency = Pa_GetDeviceInfo(p.device)->defaultHighInputLatency;
				p.hostApiSpecificStreamInfo = NULL;
				PaError err = Pa_OpenStream(&handle, &p, NULL, rec->s.rate(), 50, paClipOff, rec->c_callback, rec);
				if (err != paNoError) throw std::runtime_error("Cannot open PortAudio audio stream " + rec->s.subdev() + ": " + Pa_GetErrorText(err));
			}
			~strm() { Pa_CloseStream(handle); }
		} stream;
	  public:
		pa19_record(settings& s_orig): s(s_orig), initialize(), stream(this) {
			PaError err = Pa_StartStream(stream.handle);
			if( err != paNoError ) throw std::runtime_error("Cannot start PortAudio audio stream " + s.subdev() + ": " + Pa_GetErrorText(err));
			s_orig = s;
		}
	};
	boost::plugin::simple<record_plugin, pa19_record> r(devinfo("pa", "PortAudio v19 PCM capture. Device number as settings (otherwise PA default)."));
}
