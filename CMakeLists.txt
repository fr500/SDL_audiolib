cmake_minimum_required(VERSION 2.8)

project(SDL_audiolib)

include(CheckCXXCompilerFlag)
include(FindPkgConfig)
include(GenerateExportHeader)

option(
    USE_SDL2
    "Use SDL 2 instead of SDL 1."
    ON
)

option(
    USE_RESAMP_SRC
    "Enable SRC (aka libsamplerate) resampler."
    ON
)

option(
    USE_RESAMP_SOXR
    "Enable SoxR resampler."
    ON
)

option(
    USE_DEC_MODPLUG
    "Enable ModPlug MOD decoder."
    ON
)

option(
    USE_DEC_MPG123
    "Enable Mpg123 MP3 decoder."
    ON
)

option(
    USE_DEC_SNDFILE
    "Enable Libsndfile decoder."
    ON
)

option(
    USE_DEC_LIBVORBIS
    "Enable libvorbis Ogg Vorbis decoder."
    ON
)

option(
    USE_DEC_LIBOPUSFILE
    "Enable libopusfile Opus decoder."
    ON
)

option(
    USE_DEC_FLUIDSYNTH
    "Enable FluidSynth MIDI decoder."
    ON
)

option(
    ENABLE_SDLMIXER_EMU
    "Build the SDL_mixer emulation library (doesn't really work yet.)"
    OFF
)

option(
    WITH_BUNDLED_BOOST
    "Use the bundled version of the boost headers."
    ON
)

option(
    USE_CXX11
    "Enable C++11 mode (GCC, Clang)."
    OFF
)

if (WITH_BUNDLED_BOOST)
    include_directories("${PROJECT_SOURCE_DIR}/boost")
endif(WITH_BUNDLED_BOOST)

if(USE_CXX11)
    CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
    if(COMPILER_SUPPORTS_CXX11)
        set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
    else()
        CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
        if(COMPILER_SUPPORTS_CXX0X)
            set(CMAKE_CXX_FLAGS "-std=c++0x ${CMAKE_CXX_FLAGS}")
        else()
            message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support.")
        endif(COMPILER_SUPPORTS_CXX0X)
    endif(COMPILER_SUPPORTS_CXX11)
endif(USE_CXX11)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_CXX_FLAGS "-Wall -W -Wextra ${CMAKE_CXX_FLAGS}")
endif(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_GNUCC)

add_compiler_export_flags()

add_definitions(
    -DSPX_RESAMPLE_EXPORT=
    -DOUTSIDE_SPEEX
    -DFLOATING_POINT
    -DRANDOM_PREFIX=SDL_audiolib
)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DAULIB_DEBUG)
endif()

configure_file (
    "${PROJECT_SOURCE_DIR}/aulib_config.h.in"
    "${PROJECT_BINARY_DIR}/aulib_config.h"
)
include_directories("${PROJECT_BINARY_DIR}")

set(
    PUBLIC_HEADERS

    src/aulib_global.h
    ${CMAKE_BINARY_DIR}/aulib_export.h
    src/aulib.h
    src/audiodecoder.h
    src/audioresampler.h
    src/audiostream.h
    src/stream.h
    src/resamp_speex.h
    src/resamp_src.h
    src/resamp_sox.h
    src/dec_modplug.h
    src/dec_mpg123.h
    src/dec_sndfile.h
    src/dec_vorbis.h
    src/dec_opus.h
    src/dec_fluidsynth.h
)

if (USE_SDL2)
    pkg_check_modules(SDL REQUIRED sdl2)
else()
    pkg_check_modules(SDL REQUIRED sdl)
endif(USE_SDL2)
set(PRIVATE_LIBS ${PRIVATE_LIBS} ${SDL_LIBRARIES})

if (USE_RESAMP_SRC)
    set(
        AULIB_SOURCES ${AULIB_SOURCES}
        src/resamp_src.cpp
    )
    pkg_check_modules(SRC REQUIRED samplerate)
    set(PRIVATE_LIBS ${PRIVATE_LIBS} ${SRC_LIBRARIES})
endif(USE_RESAMP_SRC)

if (USE_RESAMP_SOXR)
    set(
        AULIB_SOURCES ${AULIB_SOURCES}
        src/resamp_sox.cpp
    )
    pkg_check_modules(SOXR REQUIRED soxr)
    set(PRIVATE_LIBS ${PRIVATE_LIBS} ${SOXR_LIBRARIES})
endif(USE_RESAMP_SOXR)

if (USE_DEC_MODPLUG)
    set(
        AULIB_SOURCES ${AULIB_SOURCES}
        src/dec_modplug.cpp
    )
    pkg_check_modules(MODPLUG REQUIRED libmodplug)
    set(PRIVATE_LIBS ${PRIVATE_LIBS} ${MODPLUG_LIBRARIES})
endif(USE_DEC_MODPLUG)

if (USE_DEC_MPG123)
    set(
        AULIB_SOURCES ${AULIB_SOURCES}
        src/dec_mpg123.cpp
    )
    pkg_check_modules(MPG123 REQUIRED libmpg123)
    set(PRIVATE_LIBS ${PRIVATE_LIBS} ${MPG123_LIBRARIES})
endif(USE_DEC_MPG123)

if (USE_DEC_SNDFILE)
    set(
        AULIB_SOURCES ${AULIB_SOURCES}
        src/dec_sndfile.cpp
    )
    pkg_check_modules(SNDFILE REQUIRED sndfile)
    set(PRIVATE_LIBS ${PRIVATE_LIBS} ${SNDFILE_LIBRARIES})
endif(USE_DEC_SNDFILE)

if (USE_DEC_LIBVORBIS)
    set(
        AULIB_SOURCES ${AULIB_SOURCES}
        src/dec_vorbis.cpp
    )
    pkg_check_modules(VORBISFILE REQUIRED vorbisfile)
    set(PRIVATE_LIBS ${PRIVATE_LIBS} ${VORBISFILE_LIBRARIES})
endif(USE_DEC_LIBVORBIS)

if (USE_DEC_LIBOPUSFILE)
    set(
        AULIB_SOURCES ${AULIB_SOURCES}
        src/dec_opus.cpp
    )
    pkg_check_modules(OPUSFILE REQUIRED opusfile)
    set(PRIVATE_LIBS ${PRIVATE_LIBS} ${OPUSFILE_LIBRARIES})
endif(USE_DEC_LIBOPUSFILE)

if (USE_DEC_FLUIDSYNTH)
    set(
        AULIB_SOURCES ${AULIB_SOURCES}
        src/dec_fluidsynth.cpp
    )
    pkg_check_modules(FLUIDSYNTH REQUIRED fluidsynth)
    set(PRIVATE_LIBS ${PRIVATE_LIBS} ${FLUIDSYNTH_LIBRARIES})
endif(USE_DEC_FLUIDSYNTH)

foreach(LIB ${PRIVATE_LIBS})
    set(PKGCONF_PRIVATE_LIBS "${PKGCONF_PRIVATE_LIBS} -l${LIB}")
endforeach()

add_library(
    SDL_audiolib

    ${PUBLIC_HEADERS}
    ${AULIB_SOURCES}

    src/aulib_debug.h
    src/aulib.cpp
    src/audiodecoder.cpp
    src/audioresampler.cpp
    src/audiostream.cpp
    src/audiostream_p.h
    src/audiostream_p.cpp
    src/resamp_speex.cpp
    src/sampleconv.h
    src/sampleconv.cpp
    src/stream.cpp
    resampler/arch.h
    resampler/resample_sse.h
    resampler/speex_resampler.h
    resampler/stack_alloc.h
    resampler/resample.c
)

set_target_properties(
    SDL_audiolib PROPERTIES VERSION 0.0.0 SOVERSION 0
)

generate_export_header(
    SDL_audiolib
    BASE_NAME aulib
)

include_directories(
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/resampler
    ${SDL_INCLUDE_DIRS}
    ${SRC_INCLUDE_DIRS}
    ${SOXR_INCLUDE_DIRS}
    ${FLUIDSYNTH_INCLUDE_DIRS}
    ${MPG123_INCLUDE_DIRS}
    ${SNDFILE_INCLUDE_DIRS}
    ${VORBISFILE_INCLUDE_DIRS}
    ${OPUSFILE_INCLUDE_DIRS}

    # MogPlug ships a "sndfile.h" with its headers. This breaks the build,
    # since sndfile.h is expected to belong to libsndfile.
    #${MODPLUG_INCLUDE_DIRS}
)

target_link_libraries(
    SDL_audiolib
    ${SDL_LIBRARIES}
    ${SRC_LIBRARIES}
    ${SOXR_LIBRARIES}
    ${FLUIDSYNTH_LIBRARIES}
    ${MODPLUG_LIBRARIES}
    ${MPG123_LIBRARIES}
    ${SNDFILE_LIBRARIES}
    ${VORBISFILE_LIBRARIES}
    ${OPUSFILE_LIBRARIES}
)

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
    set(CMAKE_INSTALL_LIBDIR lib CACHE PATH "Output directory for libraries.")
endif(NOT DEFINED CMAKE_INSTALL_LIBDIR)

configure_file(
    "${PROJECT_SOURCE_DIR}/SDL_audiolib.pc.in"
    "${PROJECT_BINARY_DIR}/SDL_audiolib.pc"
    @ONLY
)

install(TARGETS SDL_audiolib DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES ${PUBLIC_HEADERS} DESTINATION include/SDL_audiolib)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SDL_audiolib.pc"
        DESTINATION "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig")

if (ENABLE_SDLMIXER_EMU)
    add_library(
        SDL_mixer-1.2
        src/sdl_mixer_emu/sdl_mixer_emu.h
        src/sdl_mixer_emu/sdl_mixer_emu.cpp
        src/sdl_mixer_emu/music.cpp
    )

    set_target_properties(
        SDL_mixer-1.2 PROPERTIES VERSION 0.12.0 SOVERSION 0
    )

    target_link_libraries(
        SDL_mixer-1.2
        SDL_audiolib
    )

    install(TARGETS SDL_mixer-1.2 DESTINATION ${CMAKE_INSTALL_LIBDIR})
endif(ENABLE_SDLMIXER_EMU)