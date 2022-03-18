#pragma once
#ifndef POINT_H
#define POINT_H

struct Point {
    // we expect columns to come first in 2d array indexing
    // defines indexr in vertical order in 2d array
    int column;
    // defines index in horizontal order in 2d array
    int row;
};

#endif