#pragma once

#include <map>
#include <string>

#include <allegro5/allegro_audio.h>

#define SOUND_BG_MUSIC_1_ID 0x01
#define SOUND_BG_MUSIC_2_ID 0x02
#define SOUND_PLAYER_MOVE_ID 0x03

#define SONUDS_DIR "sounds\\"

using std::string;
using std::map;

typedef ALLEGRO_AUDIO_STREAM * algMusicInst;
typedef ALLEGRO_SAMPLE * algSampleInst;


class SoundPlayer
{
private:
	ALLEGRO_VOICE* alDefaultVoice;
	ALLEGRO_MIXER* alDefaultMixer;

	map<int, algMusicInst> m_audios;
	map<int, algSampleInst> m_samples;

	string GenerateSoundTempName();

public:
	SoundPlayer(void);
	~SoundPlayer(void);

	void SetSoundsVolume(float value);
	void SetMusicVolume(float value);

	void Play(int soundId, bool repeat);
	void Stop(int soundId);

	void Initialize(string currentDirectory);

	void InstallSound(int soundId, char * soundBuf, uint64_t soundSize);
	void RegisterSound(int soundId, string soundFile);
};
