//
//  nodebase.h
//  C-Ray
//
//  Created by Valtteri Koskivuori on 07/12/2020.
//  Copyright © 2020 Valtteri Koskivuori. All rights reserved.
//

#pragma once

#include "../utils/logging.h"

// Magic for comparing two nodes

struct nodeBase {
	bool (*compare)(const void *, const void *);
};

bool compareNodes(const void *A, const void *B);

//FIXME: Remove pool param, unneeded.
#define HASH_CONS(hashtable, pool, hash, T, ...) \
	{ \
		const T candidate = __VA_ARGS__; \
		const uint32_t h = hash(&candidate); \
		const T *existing = findInHashtable(hashtable, &candidate, h); \
		if (existing) {\
			logr(debug, "Reusing existing node. Args: %s\n", #__VA_ARGS__);\
			return (void *)existing; \
		} \
		logr(debug, "Inserting new node. Args: %s\n", #__VA_ARGS__); \
		insertInHashtable(hashtable, &candidate, sizeof(T), h); \
		return findInHashtable(hashtable, &candidate, h); \
	}
