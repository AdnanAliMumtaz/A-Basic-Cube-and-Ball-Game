// Assessment1.cpp: A program using the TL-Engine
//  Name:          Adnan Ali Mumtaz
//  Student ID:    2086200 / CE
#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace tle;
using namespace std;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("./Assessment1Resources");

	/**** Set up your scene here ****/

	//Varibles
	const float kSphereSpeed = 70;
	const float kRotationSpeed = 70;
	int playerPoints = 0;
	int scaling = 0;	
	float Xs, Ys, Zs;
	float sphereRadius = 10;
	float cubeXMin[13];
	float cubeXMax[13];
	float cubeYMin[13];
	float cubeYMax[13];
	float cubeZMin[13];
	float cubeZMax[13];
	float vectorX[13];
	float vectorY[13];
	float vectorZ[13];
	float distance[13];
	float distanceX[13];
	float distanceY[13];
	float distanceZ[13];

	//States--------------------------------------------------
	enum GameStates { Playing, PausedGame, GameOver, GameWon };
	GameStates gamestate = Playing;

	enum CameraStates { Top_down, Isometric };
	CameraStates camerastate = Top_down;

	enum ModesStates { Regular, HyperMode };
	ModesStates modestate = Regular;

	//IMeshs and IModels---------------------------------------
	IMesh* waterMesh = myEngine->LoadMesh("water.x");
	IModel* water = waterMesh->CreateModel(0, -5, 0);

	IMesh* islandMesh = myEngine->LoadMesh("island.x");
	IModel* island = islandMesh->CreateModel(0, -5, 0);

	IMesh* sphereMesh = myEngine->LoadMesh("spheremesh.x");
	IModel* sphere = sphereMesh->CreateModel(0, 10, 0);

	IFont* myFont = myEngine->LoadFont("none", 36);

	IMesh* cubeMesh = myEngine->LoadMesh("minicube.x");
	IModel* cube[13];
	for (int i = 0; i < 13; i++)
	{
		cube[i] = cubeMesh->CreateModel(-100 + (rand() % 200), 5, -100 + (rand() % 200));
	}
	cube[12]->SetSkin("hypercube.jpg");
	for (int i = 0; i < 13; i++) // Calculating the difference between sphere and cubes.
	{
		for (int a = 0; a < 12; a++) // Calculating the difference between sphere and cubes.
		{
			vectorX[i] = cube[a]->GetX() - sphere->GetX();
			vectorY[i] = cube[a]->GetY() - sphere->GetY();
			vectorZ[i] = cube[a]->GetZ() - sphere->GetZ();

			distanceX[i] = vectorX[i] * vectorX[i];
			distanceY[i] = vectorY[i] * vectorY[i];
			distanceZ[i] = vectorZ[i] * vectorZ[i];

			distance[i] = sqrt(distanceX[i] + distanceY[i] + distanceZ[i]);

			if (distance[i] < 11) // Generating random positions.
			{
				cube[i]->SetPosition(-100 + (rand() % 200), 5, -100 + (rand() % 200));
			}
		}

		for (int a = 0; a < 12; a++) // Calculating the difference between cubes.
		{
			vectorX[i] = cube[i]->GetX() - cube[a]->GetX();
			vectorY[i] = cube[i]->GetY() - cube[a]->GetY();
			vectorZ[i] = cube[i]->GetZ() - cube[a]->GetZ();

			distanceX[i] = vectorX[i] * vectorX[i];
			distanceY[i] = vectorY[i] * vectorY[i];
			distanceZ[i] = vectorZ[i] * vectorZ[i];

			distance[i] = sqrt(distanceX[i] + distanceY[i] + distanceZ[i]);

			if (distance[i] < 11) // Generating random positions.
			{
				cube[i]->SetPosition(-100 + (rand() % 200), 5, -100 + (rand() % 200));
			}
		}
	}

	IMesh* skyboxMesh = myEngine->LoadMesh("sky.x");
	IModel* skybox = skyboxMesh->CreateModel(0, -960, 0);

	//Camera Setup------------------------------------------
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kManual, 0, 200, 0);
	myCamera->RotateLocalX(90);

	//Timer--------------------------------------------------
	myEngine->Timer();

	//The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		//Draw the scene
		myEngine->DrawScene();
		float frameTimer = myEngine->Timer();

		/**** Update your scene each frame here ****/

		//Getting the position of sphere
		Xs = sphere->GetX();
		Ys = sphere->GetY();
		Zs = sphere->GetZ();
		
		if (gamestate == Playing || gamestate == GameWon)
		{
			//Only work when gamestate is "Playing"
			if (gamestate == Playing)
			{
				if (myEngine->KeyHeld(Key_W))
				{
					sphere->MoveLocalZ(kSphereSpeed * frameTimer);
				}
				else if (myEngine->KeyHeld(Key_S))
				{
					sphere->MoveLocalZ(-kSphereSpeed * frameTimer);
				}
				else if (myEngine->KeyHeld(Key_D))
				{
					sphere->RotateLocalY(kRotationSpeed * frameTimer);
				}
				else if (myEngine->KeyHeld(Key_A))
				{
					sphere->RotateLocalY(-kRotationSpeed * frameTimer);
				}
			}

			if (Xs >= 100 || Xs <= -100 || Zs >= 100 || Zs <= -100) //Checking for the player if it falls in the water, gamestate will be "GameOver".
			{
				gamestate = GameOver;
			}
			else if (myEngine->KeyHit(Key_P))
			{
				gamestate = PausedGame;
			}
			else if (myEngine->KeyHeld(Key_Escape))
			{
				myEngine->Stop();
			}

			else if (camerastate == Top_down) //Only works when camerastate is normal.
			{
				//Camera Moving keys
				if (myEngine->KeyHeld(Key_Up))
				{
					myCamera->MoveZ(kSphereSpeed * frameTimer);
				}
				else if (myEngine->KeyHeld(Key_Down))
				{
					myCamera->MoveZ(-kSphereSpeed * frameTimer);
				}
				else if (myEngine->KeyHeld(Key_Right))
				{
					myCamera->MoveX(kSphereSpeed* frameTimer);
				}
				else if (myEngine->KeyHeld(Key_Left))
				{
					myCamera->MoveX(-kSphereSpeed * frameTimer);
				}

				else if (myEngine->KeyHit(Key_2)) //Camera will change to Isometric.
				{
					camerastate = Isometric;
					myCamera->ResetOrientation();
					myCamera->SetPosition(150, 150, -150);
					myCamera->RotateLocalY(-45);
					myCamera->RotateLocalX(45);
				}
			}
			else if (myEngine->KeyHit(Key_1) && camerastate == Isometric) 	//Camera state changes.
			{
				myCamera->ResetOrientation();
				myCamera->SetPosition(0, 200, 0);
				myCamera->RotateLocalX(90);
				camerastate = Top_down;
			}

			for (int i = 0; i < 12; i++) //Calculating Sphere-to-Box collision detection. 
			{
				cubeXMin[i] = (cube[i]->GetX() - 2.5) - sphereRadius; 
				cubeXMax[i] = (cube[i]->GetX() + 2.5) + sphereRadius;
				cubeYMin[i] = 1 - 10;
				cubeYMax[i] = 5 + 10;
				cubeZMin[i] = (cube[i]->GetZ() - 2.5) - sphereRadius;
				cubeZMax[i] = (cube[i]->GetZ() + 2.5) + sphereRadius;

				if (Xs > cubeXMin[i] && Xs < cubeXMax[i] && Ys > cubeYMin[i] && Ys < cubeYMax[i] && Zs > cubeZMin[i] && Zs < cubeZMax[i])
				{
					cube[i]->SetPosition(-100 + (rand() % 200), 5, -100 + (rand() % 200)); 


					for (int a = 0; a < 12; a++) //Calculating the distance between the sphere and cubes. 
					{
						vectorX[i] = cube[a]->GetX() - sphere->GetX();
						vectorY[i] = cube[a]->GetY() - sphere->GetY();
						vectorZ[i] = cube[a]->GetZ() - sphere->GetZ();

						distanceX[i] = vectorX[i] * vectorX[i];
						distanceY[i] = vectorY[i] * vectorY[i];
						distanceZ[i] = vectorZ[i] * vectorZ[i];

						distance[i] = sqrt(distanceX[i] + distanceY[i] + distanceZ[i]);

						if (distance[i] < 11) //If distance is less than 10 or 10 it will generate random position.
						{
							cube[i]->SetPosition(-100 + (rand() % 200), 5, -100 + (rand() % 200));
						}
					}


					for (int a = 0; a < 12; a++) //Calculating the  distance between all cubes.
					{
						vectorX[i] = cube[a]->GetX() - cube[i]->GetX();
						vectorY[i] = cube[a]->GetY() - cube[i]->GetY();
						vectorZ[i] = cube[a]->GetZ() - cube[i]->GetZ();

						distanceX[i] = vectorX[i] * vectorX[i];
						distanceY[i] = vectorY[i] * vectorY[i];
						distanceZ[i] = vectorZ[i] * vectorZ[i];

						distance[i] = sqrt(distanceX[i] + distanceY[i] + distanceZ[i]);

						if (distance[i] < 11) //If distance is less than 10 or 10 it will generate random position.
						{
							cube[i]->SetPosition(-100 + (rand() % 200), 5, -100 + (rand() % 200));
						}
					}

					//Adding Player Points
					playerPoints += 10;
					scaling++;

					if (scaling == 4)
					{
						sphere->Scale(1.2);
						sphere->SetY(sphere->GetY()+2);
						scaling = 0;
						sphereRadius += 2;
					}
				}
				if (playerPoints == 120)
				{
					gamestate = GameWon;
					if (gamestate == GameWon)
					{
						myFont->Draw("Congratulations! You Won!", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kBlack, kCentre, kVCentre);
					}
				}
			}

			for (int i = 0; i < 2; i++)	//Calculating distance between the hypercube and other cubes.
			{
				cubeXMin[12] = (cube[12]->GetX() - 2.5) - sphereRadius;
				cubeXMax[12] = (cube[12]->GetX() + 2.5) + sphereRadius;
				cubeYMin[12] = 1 - 10;
				cubeYMax[12] = 5 + 10;
				cubeZMin[12] = (cube[12]->GetZ() - 2.5) - sphereRadius;
				cubeZMax[12] = (cube[12]->GetZ() + 2.5) + sphereRadius;

				if (Xs > cubeXMin[12] && Xs < cubeXMax[12] && Ys > cubeYMin[12] && Ys < cubeYMax[12] && Zs > cubeZMin[12] && Zs < cubeZMax[12])
				{
					//HyperCube pick up.
					cube[12]->SetPosition(10000, -500, 10000);
					modestate = HyperMode;

					if (modestate == HyperMode)
					{
						sphere->SetSkin("hypersphere.jpg");
					}
				}
			}
		}

		else if (myEngine->KeyHit(Key_P) && gamestate == PausedGame)
		{
			gamestate = Playing;
		}

		else if (gamestate == GameOver)
		{
			myFont->Draw("You lost!", myEngine->GetWidth() / 2, myEngine->GetHeight() / 2, kRed, kCentre, kVCentre);
			myFont->Draw("(Escape Key to stop the game)", myEngine->GetWidth() / 2, myEngine->GetHeight() / 1.9, kRed, kCentre, kVCentre);

			if (myEngine->KeyHeld(Key_Escape))
			{
				myEngine->Stop();
			}
		}

		//Points
		stringstream points;
		points << "Points: " << playerPoints;
		myFont->Draw(points.str(), myEngine->GetWidth() / 1.01, 0, kBlack, kRight, kTop);
		points.str("");
	}
	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}