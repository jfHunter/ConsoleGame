#include <string>
#include <map>
#include <iostream>

#include <ctime>

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include "StringUtils.hpp"

#include "SoundPlayer.h"

using std::map;
using std::string;
using std::cout;
using std::endl;

static map<int, string> gSoundMap = {
	{ SOUND_BG_MUSIC_1_ID, "come_and_find_me.ogg", },
	{ SOUND_BG_MUSIC_2_ID, "intricate_cloudy_sin.ogg", },
	{ SOUND_PLAYER_MOVE_ID, "", },
};

static string g_soundsDirectory;

void SoundPlayer::Initialize(string currentDirectory)
{
	g_soundsDirectory = currentDirectory + '\\' + SONUDS_DIR;

	//ALLEGRO_AUDIO_STREAM* stream = al_load_audio_stream("C:/Sounds/come_and_find_me.ogg", 4, 2048);
	//if (stream == NULL) {
	//	cout << "Error loading come and find me\n" << endl;
	//}
	//
	//ALLEGRO_SAMPLE* sample = al_load_sample("C:/Sounds/intricate_cloudy_sin.wav");
	//ALLEGRO_SAMPLE_INSTANCE* sampleInst = al_create_sample_instance(sample);
	//
	//al_set_audio_stream_playmode(stream, ALLEGRO_PLAYMODE_LOOP);
	//al_attach_audio_stream_to_mixer(stream, alDefaultMixer);
	//al_set_audio_stream_playing(stream, true);
	//
	//al_attach_sample_instance_to_mixer(sampleInst, alDefaultMixer);
	//al_set_sample_instance_playing(sampleInst, true);
}

void SoundPlayer::InstallSound(int soundId, char * soundBuf, uint64_t soundSize)
{
	ALLEGRO_PATH * al_temp_path = NULL;
	ALLEGRO_FILE * al_temp_file;
	ALLEGRO_AUDIO_STREAM * al_audio_stream;

	if (m_audios.find(soundId) != m_audios.end()) {
		return;
	}

	al_temp_file = al_make_temp_file("sndfile$XXXXXXXXXXXXX", &al_temp_path);

	if (al_temp_file) {
		al_fwrite(al_temp_file, soundBuf, soundSize);
		al_fclose(al_temp_file);

		al_audio_stream = al_load_audio_stream(al_path_cstr(al_temp_path, '\\'), 4, 2048);
		if (al_audio_stream) {
			m_audios.insert({ soundId, al_audio_stream });
		} else {
			cout << "Failed to load audio stream" << endl;
		}
	} else {
		cout << "Failed to create temp random file" << endl;
	}
}

void SoundPlayer::RegisterSound(int soundId, string soundFile)
{
	ALLEGRO_SAMPLE * al_sample = al_load_sample(String(g_soundsDirectory + '\\' + soundFile).c_str());

	if (al_sample) {
		m_samples.insert({ soundId, al_sample });
	} else {
		cout << "Failed to load sample" << endl;
	}
}

SoundPlayer::SoundPlayer(void)
{
	if (!al_install_system(ALLEGRO_VERSION_INT, atexit)) {
		std::cout << "Failed to init ALLEGRO system\n";
	}

	if (!al_install_audio()) {
		std::cout << "Failed to init ALLEGRO audio subsystem\n";
	}

	if (!al_init_acodec_addon()) {
		std::cout << "Failed to init audio codec addon\n";
	}

	alDefaultVoice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
	alDefaultMixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);

	al_attach_mixer_to_voice(alDefaultMixer, alDefaultVoice);
}

SoundPlayer::~SoundPlayer(void)
{
	al_uninstall_audio();
	al_uninstall_system();
}

void SoundPlayer::Play(int soundId, bool repeat)
{

}

void SoundPlayer::Stop(int soundId)
{

}
