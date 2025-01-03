#ifndef LOADERS_HH
#define LOADERS_HH

#include "settings.hh"
#include <raylib.h>

namespace RaylibInitializer {

class CustomStd {
public:
  static int strLength(const char *str);
  static void intToStr(unsigned int num, char *buffer);
  static char *concatenate(const char *str, int num);
};

class Models {
public:
  Model red_signal, green_signal, terrain, tree, track, train, train_station,
      sign, electricity, electricity_r, track_bent, track_bent_r, mountains,
      grass;
  Models();
  ~Models();
};

class Musics {
public:
  Music train_background_protagonist, train_background_dummy, horn;
  Musics();
  ~Musics();
};

class BasicInitializers {
public:
  BasicInitializers();
  ~BasicInitializers();
};

class Rules {
public:
  bool stop_at_next_station;
  unsigned short iteration; // How many stations have been crossed
  bool within_station_boundary;
  bool failed;
  unsigned short score;
  bool honked;
  bool show_instructions;
  Rules();
};

class Cameras {
public:
  int which_active_camera;
  Camera3D active_camera;
  Camera3D front_camera;
  Camera3D top_view_camera;
  Cameras();
  void switch_camera();
  void update_active_camera(float protagonist_train_speed);
};

class TrainConfig {
public:
  Vector3 protagonist_train_position, dummy_train_position;
  float protagonist_train_speed;
  TrainConfig();
};

class InitRaylib {
  const BasicInitializers _;

public:
  const Models models;
  const Musics musics;
  Rules rules;
  Cameras cameras;
  TrainConfig train_config;
};

} // namespace RaylibInitializer
#endif