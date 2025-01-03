#include "loaders.hh"
#include "settings.hh"
#include <raylib.h>

using namespace RaylibInitializer;
// ==========================
// CustomStd Implementation
// ==========================
int CustomStd::strLength(const char *str) {
  int len = 0;
  while (str[len] != '\0') {
    ++len;
  }
  return len;
}

void CustomStd::intToStr(unsigned int num, char *buffer) {
  int i = 0;
  do {
    buffer[i++] = (num % 10) + '0'; // Get each digit
    num /= 10;
  } while (num > 0);
  buffer[i] = '\0'; // Null-terminate the string

  // Reverse the buffer
  for (int j = 0, k = i - 1; j < k; ++j, --k) {
    char temp = buffer[j];
    buffer[j] = buffer[k];
    buffer[k] = temp;
  }
}

char* CustomStd::concatenate(const char *str, int num) {
    int strLen = strLength(str);
    char numBuffer[20];
    intToStr(num, numBuffer);
    int numLen = strLength(numBuffer);

    // Allocate memory dynamically
    char* result = new char[strLen + numLen + 1];

    for (int i = 0; i < strLen; ++i) result[i] = str[i];
    for (int i = 0; i < numLen; ++i) result[strLen + i] = numBuffer[i];
    result[strLen + numLen] = '\0';
    return result;
}


// ==========================
// Models Implementation
// ==========================
Models::Models() {
  red_signal = LoadModel("./assets/red_signal.glb");
  green_signal = LoadModel("./assets/green_signal.glb");
  terrain = LoadModel("./assets/terrain.glb");
  tree = LoadModel("./assets/tree.glb");
  track = LoadModel("./assets/track.glb");
  train = LoadModel("./assets/train.glb");
  train_station = LoadModel("./assets/train_station.glb");
  sign = LoadModel("./assets/sign.glb");
  electricity = LoadModel("./assets/electricity.glb");
  electricity_r = LoadModel("./assets/electricity_r.glb");
  track_bent = LoadModel("./assets/track_bent.glb");
  track_bent_r = LoadModel("./assets/track_bent_r.glb");
  mountains = LoadModel("./assets/mountains.glb");
  grass = LoadModel("./assets/scene.glb");
}

Models::~Models() {
  UnloadModel(red_signal);
  UnloadModel(green_signal);
  UnloadModel(terrain);
  UnloadModel(tree);
  UnloadModel(track);
  UnloadModel(train);
  UnloadModel(train_station);
  UnloadModel(sign);
  UnloadModel(electricity);
  UnloadModel(electricity_r);
  UnloadModel(track_bent);
  UnloadModel(track_bent_r);
  UnloadModel(mountains);
  UnloadModel(grass);
}

// ==========================
// Musics Implementation
// ==========================
Musics::Musics() {
  train_background_protagonist = LoadMusicStream("./music/train.mp3");
  train_background_dummy = LoadMusicStream("./music/train.mp3");
  horn = LoadMusicStream("./music/horn.mp3");
}

Musics::~Musics() {
  UnloadMusicStream(train_background_protagonist);
  UnloadMusicStream(train_background_dummy);
  UnloadMusicStream(horn);
}

// ==========================
// BasicInitializers Implementation
// ==========================
BasicInitializers::BasicInitializers() {
  InitAudioDevice();
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Model Loader");
  SetTargetFPS(FPS);
}

BasicInitializers::~BasicInitializers() {
  CloseWindow();
  CloseAudioDevice();
}

// ==========================
// Rules Implementation
// ==========================
Rules::Rules() {
  stop_at_next_station = false;
  iteration = 0;
  within_station_boundary = false;
  failed = false;
  score = 0;
  honked = false;
  show_instructions = true;
}

// ==========================
// Cameras Implementation
// ==========================
Cameras::Cameras() {
  front_camera = (Camera3D){
      (Vector3){20, 4, 4}, (Vector3){20, -1.8, 10000},
      (Vector3){0, 1, 0},  60.0f,
      CAMERA_PERSPECTIVE,
  };

  top_view_camera = (Camera3D){
      (Vector3){5, 20, 4}, (Vector3){20, 4, 14}, (Vector3){0, 1, 0}, 60.0,
      CAMERA_PERSPECTIVE,
  };

  which_active_camera = 0;
  active_camera = front_camera;
}

void Cameras::switch_camera() {
  active_camera = (which_active_camera == 0) ? top_view_camera : front_camera;
  which_active_camera = 1 - which_active_camera;
}

void Cameras::update_active_camera(float protagonist_train_speed) {
  front_camera.position.z += protagonist_train_speed;
  top_view_camera.position.z = front_camera.position.z + 50;
  top_view_camera.target = front_camera.position;
  if (which_active_camera == 0) {
    active_camera = front_camera;
  } else {
    active_camera = top_view_camera;
  }
}

// ==========================
// TrainConfig Implementation
// ==========================
TrainConfig::TrainConfig() {
  protagonist_train_position = (Vector3){20, 1.5, 10};
  dummy_train_position = (Vector3){11.2, 1.5, 200};
  protagonist_train_speed = 0;
}
// InitRaylib()
//     : models(), musics(), rules(), cameras(), train_config(), _() {}
 // namespace RaylibInitializer