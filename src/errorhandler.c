//
//  errorhandler.c
//  C-Ray
//
//  Created by Valtteri Koskivuori on 14/09/15.
//  Copyright (c) 2015 Valtteri Koskivuori. All rights reserved.
//

#include "errorhandler.h"

//Prototype
void logr(const char *log, logSource source);

void logHandler(renderLog error) {
    switch (error) {
        case threadMallocFailed:
            logr("Failed to allocate memory for thread args, aborting.", renderer);
			exit(-1);
            break;
        case imageMallocFailed:
            logr("Failed to allocate memory for image data, aborting.", renderer);
            exit(-1);
            break;
        case sceneBuildFailed:
            logr("Scene builder failed. (Missing scene file.) Aborting.", sceneBuilder);
            exit(-1);
            break;
        case invalidThreadCount:
            logr("Render sections and thread count are not even. Render will be corrupted (likely).", renderer);
            break;
        case threadFrozen:
            logr("A thread has frozen. Aborting.", renderer);
            exit(-1);
            break;
        case defaultError:
            logr("Something went wrong. Aborting.", defaultSource);
            break;
        case sceneDebugEnabled:
            logr("SceneBuilder returned debug flag, won't render this.", renderer);
            break;
        case sceneParseErrorScene:
            logr("SceneBuilder failed to parse the scene block.", sceneBuilder);
            break;
        case sceneParseErrorCamera:
            logr("SceneBuilder failed to parse the camera block.", sceneBuilder);
            break;
        case sceneParseErrorSphere:
            logr("SceneBuilder failed to parse the sphere block.", sceneBuilder);
            break;
        case sceneParseErrorPoly:
            logr("SceneBuilder failed to parse the polygon block.", sceneBuilder);
            break;
        case sceneParseErrorLight:
            logr("SceneBuilder failed to parse the light block.", sceneBuilder);
            break;
		case sceneParseErrorMaterial:
			logr("SceneBuilder failed to parse the material block.", sceneBuilder);
			break;
        case sceneParseErrorMalloc:
            logr("SceneBuilder failed to parse the scene for malloc", sceneBuilder);
            break;
        case sceneParseErrorNoPath:
            logr("No input file path given!", sceneBuilder);
            break;
		case dontTurnOnTheAntialiasingYouDoofus:
			logr("You fucked up.", renderer);
			exit(-1);
			break;
		case renderErrorInvalidSampleCount:
			logr("Samples set to less than 1, aborting.", renderer);
			exit(-1);
			break;
		case drawTaskMallocFailed:
			logr("Failed to allocate memory for UI draw tasks", renderer);
			exit(-1);
			break;
        default:
            logr("Something went wrong. Aborting.", defaultSource);
            exit(-1);
            break;
    }
}

void logr(const char *log, logSource source) {
    switch (source) {
        case renderer:
            printf("RENDERER: ");
            break;
        case sceneBuilder:
            printf("SCNBUILDER: ");
            break;
        case vectorHandler:
            printf("VECHANDLER: ");
            break;
        case colorHandler:
            printf("COLRHANDLER: ");
            break;
        case polyHandler:
            printf("POLYHANDLER: ");
            break;
        case sphereHandler:
            printf("SPHRHANDLER: ");
            break;
        case fileHandler:
            printf("FLHANDLER: ");
            break;
        default:
            printf("LOG: ");
            break;
    }
    printf("%s\n",log);
}
