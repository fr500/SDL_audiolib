/*
  SDL_audiolib - An audio decoding, resampling and mixing library
  Copyright (C) 2014  Nikos Chantziaras

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/
#ifndef RESAMP_SRC_H
#define RESAMP_SRC_H

#include <Aulib/AudioResampler.h>

namespace Aulib {

/*!
 * \brief SRC (libsamplerate) resampler.
 */
class AULIB_EXPORT AudioResamplerSRC final: public AudioResampler {
public:
    AudioResamplerSRC();
    ~AudioResamplerSRC() override;

protected:
    void doResampling(float dst[], const float src[], int& dstLen, int& srcLen) override;

    int adjustForOutputSpec(int dstRate, int srcRate, int channels) override;

private:
    const std::unique_ptr<struct AudioResamplerSRC_priv> d;
};

} // namespace Aulib

#endif
