#include <stdio.h>

#define TRIANGLE_PARAMETERS 12
#define ID_PARAMETERS 1
#define RAY_PARAMETERS 6


#define MAX_TRIANGLES   1000000
#define MAX_RAYS        1920*1080

#define data_t float



void intersect_triangles(
    int triangle_index,
    data_t* triangles,
    int num_triangles,
    int ray_index,
    data_t* rays,
    int num_rays,
    int* min_intersection_ids,
    data_t* min_ts
    )
{
    /// min_t   <- infinity
    /// min_id  <- -1
    /// hit
    /// for i <- 1 to num_triangles
    ///     hit <- 0
    ///     calculate ray-triangle intersect on rays[ray_index]-triangles[i]
    ///     if(hit)
    ///         if (t < min_t)
    ///             min_id <- id
    ///             min_t  <- t
    ///
    /// min_intersection_ids[ray_index] <- min_id
    /// min_ts <- min id
    ///
}



