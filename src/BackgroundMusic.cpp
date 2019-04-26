#include <iostream>
#include "BackgroundMusic.hpp"
#include "Resource.hpp"

void BackgroundMusic::Step() {
  if (time_ >= 1.f)
    return;
  time_ += 0.01f;

  if (time_ >= 1.f) {
    foreground_sound_.SetVolume(1.f);
    background_sound_.Stop();
  } else {
    foreground_sound_.SetVolume(time_);
    background_sound_.SetVolume(1.f - time_);
  }
}

void BackgroundMusic::SetSound(const smk::SoundBuffer& buffer) {
  if (&buffer == foreground_sound_.buffer())
    return;
  background_sound_ = std::move(foreground_sound_);
  foreground_sound_ = smk::Sound();
  foreground_sound_.SetBuffer(buffer);
  foreground_sound_.SetLoop(true);
  foreground_sound_.SetVolume(0.f);
  foreground_sound_.Play();
  time_ = 0.f;
}
