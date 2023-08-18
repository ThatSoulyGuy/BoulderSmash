#ifndef SOUND_MANAGER_HPP

#endif // !SOUND_MANAGER_HPP


#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <glm/vec3.hpp>
#include <SND/sndfile.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <GLFW/glfw3.h>
#include "core/Logger.hpp"

struct SoundEffect
{
    float pitch = 1.0f;

    std::string name;
    std::string path;

	glm::vec3 positon = glm::vec3{ 0.0f, 0.0f, 0.0f };
    glm::vec3 velocity = glm::vec3{ 0.0f, 0.0f, 0.0f };

    ALuint source = 0;
    ALuint sound = 0;

	static SoundEffect Register(const std::string& path, const std::string& name)
	{
		SoundEffect out;

		out.pitch = 1.1f;
		
		out.name = name;
		out.path = "assets/" + path + ".mp3";

		out.positon = glm::vec3{ 0.0f, 0.0f, 0.0f };
		out.velocity = glm::vec3{ 0.0f, 0.0f, 0.0f };
		
		return out;
	}
};

ALCdevice* device;
ALCcontext* context;
std::vector<SoundEffect> soundBuffers;

namespace SoundManager
{
    void Init()
    {
        device = alcOpenDevice(nullptr);
        context = alcCreateContext(device, nullptr);

        if (!device)
            Logger_ThrowError("NULL", "Falied to get Sound Device", true);

        if (!context)
            Logger_ThrowError("NULL", "Failed to get Sound Context", true);

        if (!alcMakeContextCurrent(context))
            Logger_ThrowError("NULL", "Falied to make Sound Context current", true);

        const ALCchar* name = nullptr;
        if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
            name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
        if (!name || alcGetError(device) != AL_NO_ERROR)
            name = alcGetString(device, ALC_DEVICE_SPECIFIER);

        Logger_WriteConsole("Successfully opened Sound Device: '" + std::string(name) + "'", LogLevel::INFO);
    }

    void CleanUp()
    {
        for (auto& sound : soundBuffers)
            alDeleteBuffers(1, &sound.sound);
        
        alcMakeContextCurrent(NULL);
        alcDestroyContext(context);
        alcCloseDevice(device);
    }

    void RegisterSound(SoundEffect effect) 
    {
		ALenum error, format = AL_NONE;
		ALuint buffer = 0;

		SNDFILE* file;
		SF_INFO info;

		short* memoryBuffer;

		sf_count_t numberFrames;
		ALsizei numberBytes;

		file = sf_open(effect.path.c_str(), SFM_READ, &info);

		if (!file)
		{
			Logger_ThrowError("NULL", "Failed to open Audio at: '" + effect.path + "'.", false);
			return;
		}

		if (info.frames < 1 || info.frames >(sf_count_t)(INT_MAX / sizeof(short)) / info.channels)
		{
			Logger_ThrowError("info.frames < 1", "Bad Audio sample count at: '" + effect.path + "'.", false);
			sf_close(file);
			return;
		}


		if (info.channels == 1)
			format = AL_FORMAT_MONO16;
		else if (info.channels == 2)
			format = AL_FORMAT_STEREO16;
		else if (info.channels == 3)
		{
			if (sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (info.channels == 4)
		{
			if (sf_command(file, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT3D_16;
		}

		if (!format)
		{
			Logger_ThrowError("NULL", "Unexpected Audio channel count: '" + std::to_string(info.channels) + "'.", false);
			sf_close(file);
			return;
		}

		memoryBuffer = static_cast<short*>(malloc((size_t)(info.frames * info.channels) * sizeof(short)));

		numberFrames = sf_readf_short(file, memoryBuffer, info.frames);

		if (numberFrames < 1)
		{
			Logger_ThrowError("NULL", "Failed to read Audio samples in: '" + effect.path + "'.", false);
			free(memoryBuffer);
			sf_close(file);
			return;
		}

		numberBytes = (ALsizei)(numberFrames * info.channels) * (ALsizei)sizeof(short);

		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, memoryBuffer, numberBytes, info.samplerate);

		free(memoryBuffer);  
		sf_close(file);

		error = alGetError();

		if (error != AL_NO_ERROR)
		{
			Logger_ThrowError(std::to_string(error), "OpenAL error: '" + std::string(alGetString(error)) + "'.", false);
			if (buffer && alIsBuffer(buffer))
			{
				alDeleteBuffers(1, &buffer);
				return; 
			}
		}

		effect.sound = buffer;

		soundBuffers.push_back(effect);
    }

    void PlayEffect(const std::string& name, const bool& loop, const float& volume = 1.1f)
    {
		for (auto effect : soundBuffers)
		{
			if (effect.name == name)
			{
				ALuint source;
				alGenSources(1, &source);
				alSourcef(source, AL_PITCH, effect.pitch);
				alSourcef(source, AL_GAIN, volume);
				alSource3f(source, AL_POSITION, effect.positon.x, effect.positon.y, effect.positon.z);
				alSource3f(source, AL_VELOCITY, effect.velocity.x, effect.velocity.y, effect.velocity.z);
				alSourcei(source, AL_LOOPING, loop);
				alSourcei(source, AL_BUFFER, (ALint)effect.sound);

				alSourcePlay(source);
			}
		}
    }
};