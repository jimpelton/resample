//
// Created by Jim Pelton on 2/28/16.
//

#ifndef RESAMPLE_GRID_H
#define RESAMPLE_GRID_H


#include <glm/glm.hpp>

template<typename T>
class Grid
{
public:
  size_t nx, ny, nz; // number of vertices
  size_t nvoxels ; // number of voxels (cube)
  const T *data;

  Grid(size_t nvx, size_t nvy, size_t nvz, const T *data)
      : nx(nvx)
      , ny(nvy)
      , nz(nvz)
      , nvoxels(nvx * nvy * nvz)
      , data(data) { }

  ~Grid() { }

  unsigned IX(unsigned x, unsigned y, unsigned z)
  {
    if (!(x < nx))
      x -= 1;

    if (!(y < ny))
      y -= 1;

    if (!(z < nz))
      z -= 1;
    return x * nx * ny + y * nx + z;
  }

  T
  bilinear
      (
          const T &tx,
          const T &ty,
          const T &c00,
          const T &c10,
          const T &c01,
          const T &c11
      )
  {
    T a = c00 * (T(1) - tx) + c10 * tx;
    T b = c01 * (T(1) - tx) + c11 * tx;
    return a * (T(1) - ty) + b * ty;
  }

  T
  interpolate
      (
          const glm::vec3& location
      )
  {
    T gx, gy, gz;
    T tx, ty, tz;
    unsigned gxi, gyi, gzi;

    // remap point coordinates to grid coordinates
    gx = location.x * nvoxels;
    gxi = static_cast<unsigned int>(gx);
    tx = gx - gxi;

    gy = location.y * nvoxels;
    gyi = static_cast<unsigned int>(gy);
    ty = gy - gyi;

    gz = location.z * nvoxels;
    gzi = static_cast<unsigned int>(gz);
    tz = gz - gzi;

    const T & c000 = data[IX(gxi, gyi, gzi)];
    const T & c100 = data[IX(gxi + 1, gyi, gzi)];
    const T & c010 = data[IX(gxi, gyi + 1, gzi)];
    const T & c110 = data[IX(gxi + 1, gyi + 1, gzi)];
    const T & c001 = data[IX(gxi, gyi, gzi + 1)];
    const T & c101 = data[IX(gxi + 1, gyi, gzi + 1)];
    const T & c011 = data[IX(gxi, gyi + 1, gzi + 1)];
    const T & c111 = data[IX(gxi + 1, gyi + 1, gzi + 1)];

    T e = bilinear(tx, ty, c000, c100, c010, c110);
    T f = bilinear(tx, ty, c001, c101, c011, c111);
    return e * ( 1 - tz) + f * tz;
  }
};




#endif //RESAMPLE_GRID_H
