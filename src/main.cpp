#include <raylib.h>
#include "libs/loaders.hh"


int main()
{
    RaylibInitializer::InitRaylib rl;
    PlayMusicStream(rl.musics.train_background_protagonist);
    PlayMusicStream(rl.musics.train_background_dummy);
    PlayMusicStream(rl.musics.horn);

    while (!WindowShouldClose())
    {
        UpdateCamera(&rl.cameras.active_camera, CAMERA_PERSPECTIVE);
        BeginDrawing();
        {
            ClearBackground(SKYBLUE);
            BeginMode3D(rl.cameras.active_camera);
            {
                {
                    rl.cameras.update_active_camera(rl.train_config.protagonist_train_speed);

                    if (rl.train_config.protagonist_train_speed > 0.0f)
                    {
                        UpdateMusicStream(rl.musics.train_background_protagonist);
                        if (rl.train_config.protagonist_train_speed < 1)
                        {
                            SetMusicPitch(rl.musics.train_background_protagonist, rl.train_config.protagonist_train_speed);
                        }
                        SetMusicPan(rl.musics.train_background_protagonist, 0.5);
                        rl.train_config.protagonist_train_speed -= 0.001f;
                    }

                    if (rl.train_config.protagonist_train_position.z < rl.train_config.dummy_train_position.z + 100 && rl.train_config.protagonist_train_position.z > rl.train_config.dummy_train_position.z - 350)
                    {
                        UpdateMusicStream(rl.musics.train_background_dummy);
                        SetMusicPitch(rl.musics.train_background_dummy, 1);
                        SetMusicPan(rl.musics.train_background_dummy, rl.cameras.which_active_camera);
                    }

                    if (IsKeyPressed(KEY_C))
                    {
                        rl.cameras.switch_camera();
                    }
                    if (IsKeyDown(KEY_H))
                    {
                        UpdateMusicStream(rl.musics.horn);
                    }
                    else if (IsKeyReleased(KEY_H))
                    {
                        SeekMusicStream(rl.musics.horn, 0.0f);
                    }

                    if (IsKeyDown(KEY_W) && rl.train_config.protagonist_train_speed < 5.1f)
                    {
                        rl.train_config.protagonist_train_speed += ACCELERATION;
                    }
                    else if (rl.train_config.protagonist_train_speed > 0.0f && IsKeyDown(KEY_B))
                    {
                    }
                    else if (IsKeyDown(KEY_S))
                    {
                        rl.train_config.protagonist_train_speed -= 0.005f;
                    }

                    if (rl.cameras.front_camera.position.z > 2000.0f)
                    {
                        rl.cameras.front_camera.position.z = 0.0f;
                        rl.rules.iteration += 1.0f;
                    }
                    if (rl.cameras.front_camera.position.z > 1967 or rl.cameras.front_camera.position.z < 7)
                    {
                        rl.rules.within_station_boundary = !rl.rules.within_station_boundary;
                    }

                    rl.rules.stop_at_next_station = rl.rules.iteration != 0 && rl.rules.iteration % INTERVALS_FOR_STOPS == 0;

                    if (rl.rules.stop_at_next_station && rl.rules.within_station_boundary && rl.train_config.protagonist_train_speed <= 0)
                    {
                        rl.rules.stop_at_next_station = false; // after 1 station
                        rl.rules.iteration = 0;
                        // WIN
                    }
                    if (rl.rules.stop_at_next_station && rl.cameras.front_camera.position.z > 1990 && rl.train_config.protagonist_train_speed != 0)
                    {
                        rl.rules.failed = true;
                    }
                    if (IsKeyPressed(KEY_I))
                    {
                        rl.rules.show_instructions = !rl.rules.show_instructions;
                    }
                    if (IsKeyDown(KEY_H) and rl.rules.within_station_boundary and !rl.rules.honked)
                    {
                        rl.rules.honked = true;
                        rl.rules.score += 100;
                    }

                    if (!rl.rules.within_station_boundary)
                    {
                        rl.rules.honked = false;
                    }
                }
                {
                    for (float z = -20.0f; z < 100.0f; z += 1.0f) // 'z' loop moved outside
                    {
                        for (float x = -5.0f; x < 4.0f; x += 1.0f) // 'x' loop inside
                        {
                            if (x == 0 || x == 1)
                                continue;

                            const float mul = z * 40.0f;
                            // Save Memory!!
                            if (mul < rl.cameras.active_camera.position.z - RENDER_DISTANCE_BACK_FOR_TREES || mul > rl.cameras.active_camera.position.z + RENDER_DISTANCE_FRONT_FOR_TREES)
                                continue;
                            DrawModel(rl.models.tree, (Vector3){x * 20, 12, mul}, 0.3f, BROWN);
                        }
                    }

                    for (float forMountains = -5; forMountains < 15; forMountains += 1)
                    {
                        const float mul = forMountains * 200;
                        DrawModel(rl.models.mountains, (Vector3){-300, 30, mul}, 2.0f, GRAY);
                        DrawModel(rl.models.mountains, (Vector3){150, 30, mul}, 2.0f, GRAY);
                    }

                    for (float i = -30; i < 290; i += 1)
                    {
                        const float mul = i * 8.0f;
                        // Save Memory!!!
                        if (mul < rl.cameras.active_camera.position.z - RENDER_DISTANCE_BACK || mul > rl.cameras.active_camera.position.z + RENDER_DISTANCE_FRONT)
                            continue;
                        DrawModel(rl.models.track, (Vector3){20.7, 0.62, mul}, 0.01f, GRAY);
                        DrawModel(rl.models.track, (Vector3){10.7, 0.62, mul}, 0.01f, GRAY);
                    }

                    // rl.drawModel(models.sky, rl.Vector3.init(protagonist_train_position.x, protagonist_train_position.y + 60.0, protagonist_train_position.z + 100.0), 500, rl.Color.light_gray);

                    DrawModel(rl.models.train, rl.train_config.dummy_train_position, 1.7f, YELLOW);
                    DrawModel(rl.models.train, rl.train_config.protagonist_train_position, 1.7f, RAYWHITE);
                    rl.train_config.protagonist_train_position.x = rl.cameras.front_camera.position.x + 1.55;
                    rl.train_config.protagonist_train_position.z = rl.cameras.front_camera.position.z - 100;
                    if (rl.train_config.dummy_train_position.z < -50)
                        rl.train_config.dummy_train_position.z = 2050;

                    rl.train_config.dummy_train_position.z -= 0.5;

                    for (float forElectricity = -20; forElectricity < 50; forElectricity += 1)
                    {
                        const float mul = forElectricity * 50;
                        // Save Memory!!!
                        if (mul < rl.cameras.active_camera.position.z - 700 || mul > rl.cameras.active_camera.position.z + 350)
                            continue;
                        DrawModel(rl.models.electricity, (Vector3){26, 4, mul + 10.0f}, 0.5f, GRAY);
                        DrawModel(rl.models.electricity_r, (Vector3){5.3, 4, mul}, 0.5f, GRAY);
                    }

                    {
                        DrawModel(rl.models.sign, (Vector3){30, 2, 200}, 0.1f, GRAY);
                        for (int z = 1; z < 4; ++z)
                        {
                            DrawModel(rl.models.sign, (Vector3){30, 2, z * 550.0f}, 0.1f, GRAY);
                        }
                    }

                    DrawModel(rl.models.train_station, (Vector3){35, 3.4, -9}, 0.3f, GRAY);
                    DrawModel(rl.models.train_station, (Vector3){35, 3.4, 1990}, 0.3f, GRAY);

                    if (rl.rules.stop_at_next_station)
                    {
                        DrawModel(rl.models.red_signal, (Vector3){28, 2.4, 2040}, 0.1f, GRAY);
                        DrawModel(rl.models.red_signal, (Vector3){28, 2.4, 40}, 0.1f, GRAY);
                    }
                    else
                    {
                        DrawModel(rl.models.green_signal, (Vector3){28, 2.4, 2040}, 0.1f, GRAY);
                        DrawModel(rl.models.green_signal, (Vector3){28, 2.4, 40}, 0.1f, GRAY);
                    }

                    DrawModel(rl.models.track_bent_r, (Vector3){15.7, 1.7, 50}, 0.15f, DARKGRAY);
                    DrawModel(rl.models.track_bent, (Vector3){15.7, 1.7, 1720}, 0.15f, DARKGRAY);

                    // rl.drawCube(rl.Vector3.init(20.8, 0.1, 0.0), 6, 0.01, 7000, DARKGRAY);
                    DrawCube((Vector3){21, 9, 1000}, 0.1f, 0.1f, 2500, BLACK);
                    DrawCube((Vector3){21, 11.3, 1000}, 0.1f, 0.1f, 2500, BLACK);
                    DrawCube((Vector3){10.5, 9, 1000}, 0.1f, 0.1f, 2500, BLACK);
                    DrawCube((Vector3){10.5, 11.3, 1000}, 0.1f, 0.1f, 2500, BLACK);

                    for (float forGrass = -30; forGrass < 130; forGrass += 1)
                    {
                        float forGrassx = -40;
                        const float forGrassZ = forGrass * 20; // compute this once, use multiple times.
                        if (forGrassZ < rl.cameras.active_camera.position.z - RENDER_DISTANCE_BACK || forGrassZ > rl.cameras.active_camera.position.z + RENDER_DISTANCE_FRONT)
                            continue;
                        for (forGrassx = -40; forGrassx < 100; forGrassx += 20)
                        {
                            DrawModel(rl.models.grass, (Vector3){forGrassx, 0.1, forGrassZ}, 1.0f, GRAY);
                        }
                    }
                }
            }
            EndMode3D();
            if (rl.rules.show_instructions)
            {
                DrawRectangle(10, 10, 250, 130, SKYBLUE);
                DrawRectangleLines(10, 10, 250, 130, BLUE);
                DrawText("Train controls:", 20, 20, 10, BLACK);
                DrawText("- Go forward: W, Go back : S", 40, 40, 10, DARKGRAY);
                DrawText("- Press H to Honk, Press B for breaks", 40, 60, 10, DARKGRAY);
                DrawText("- Press C to change camera View", 40, 80, 10, DARKGRAY);
                DrawText("- Honk at stations to increase score", 40, 100, 10, DARKGRAY);
                DrawText("- Press I to hide these instructions", 40, 120, 10, DARKGRAY);
            }

            if (IsKeyPressed(KEY_W) && rl.train_config.protagonist_train_speed > SPEED_LIMIT)
            {
                DrawText("Max Speed", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, 20, RED);
            }
            if (rl.rules.failed)
            {
                DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, DARKGRAY);
                DrawRectangleLines(10, 10, 250, 70, DARKGRAY);
                DrawText("Failed", SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 200, RED);
            }

            DrawText(RaylibInitializer::CustomStd::concatenate("Score: ", rl.rules.score), SCREEN_WIDTH - 220, 10, 20, BLACK);
            DrawText(RaylibInitializer::CustomStd::concatenate("Speed: ", rl.train_config.protagonist_train_speed * 2 * 10), SCREEN_WIDTH - 220, 30, 20, BLACK);
            DrawText(RaylibInitializer::CustomStd::concatenate("Next station: ", 2000 - rl.cameras.front_camera.position.z), SCREEN_WIDTH - 220, 50, 20, BLACK);

            if (rl.rules.stop_at_next_station)
            {
                DrawText("Status: Stop at", SCREEN_WIDTH - 220, 70, 20, RED);
                DrawText("          next station", SCREEN_WIDTH - 220, 90, 20, RED);
                DrawText("Stop at Next station", SCREEN_WIDTH - 500, 300, 25, RED);
            }
            else
            {
                DrawText("Status: Don't stop", SCREEN_WIDTH - 220, 70, 20, GREEN);
            }

            if (rl.cameras.front_camera.position.z < 0)
            {
                rl.cameras.front_camera.position.z = 0.1;
                rl.train_config.protagonist_train_speed = 0;
                DrawText("Wrong Direction", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 100, 20, RED);
            }
        }
        EndDrawing();
    }
    return 0;
}
