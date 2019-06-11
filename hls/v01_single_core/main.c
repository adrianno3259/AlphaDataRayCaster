#include "main.h"

/*
 * #define NUM_TRIS 2000
 * #define NUM_RAYS 40000
 * #define TRI_PAR 12
 * #define RAY_PAR 6
 * */

 #define MAX_TRIS 2000
 #define MAX_RAYS 40000
 #define TRI_PAR 9
 #define RAY_PAR 6


//double local_tData[MAX_TRIS * TRI_PAR];

void intersectFPGA(
	int i_tNumber,           /// number of triangles
	volatile double *i_tData, /// triangle information.    Size = i_tNumber * Triangle attribute number(12)
	volatile int *i_tIds,    /// triangle id information. Size = i_tNumber
	int i_rNumber,           /// number of rays
	volatile double *i_rData, /// ray information          Size = i_rNumber * Ray Attr number(6)
	volatile int *o_tIds,     /// min intersection triangle ids for each ray. Size = i_rNumber
	volatile double *o_tIntersects   /// min intersection t value for each ray       Size = i_rNumber
	)
{

//#pragma HLS ARRAY_PARTITION variable=local_tData block factor=100 dim=1
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS INTERFACE s_axilite port=i_tData bundle=control
#pragma HLS INTERFACE m_axi depth=600000 port=i_tData //bundle=data

#pragma HLS INTERFACE s_axilite port=i_tIds bundle=control
#pragma HLS INTERFACE m_axi depth=50000 port=i_tIds //bundle=data

#pragma HLS INTERFACE s_axilite port=i_rData bundle=control
#pragma HLS INTERFACE m_axi depth=12441600 port=i_rData //bundle=data

#pragma HLS INTERFACE s_axilite port=o_tIds bundle=control
#pragma HLS INTERFACE m_axi depth=2073600 port=o_tIds //bundle=data

#pragma HLS INTERFACE s_axilite port=o_tIntersects bundle=control
#pragma HLS INTERFACE m_axi depth=2073600 port=o_tIntersects // bundle=data

#pragma HLS INTERFACE s_axilite port=i_tNumber bundle=control
#pragma HLS INTERFACE s_axilite port=i_rNumber bundle=control

	const int triangleAttributeNumber = 9; /// p1, p2, p3 and normal * 3
    const int rayAttributeNumber = 6;       /// origin and direction  * 3
    const double EPSILON = 0.001;
    const double infinity = 1000000000.0;

    //memcpy(local_tData, i_tData, i_tNumber*TRI_PAR);

    /*
    double tris[MAX_TRIS * TRI_PAR], rays[MAX_RAYS * RAY_PAR];
	#pragma HLS ARRAY_PARTITION variable=rays cyclic factor=6 dim=1
	#pragma HLS ARRAY_PARTITION variable=tris cyclic factor=9 dim=1
    memcpy(tris, i_tData, sizeof(double) * i_tNumber * TRI_PAR);
    memcpy(rays, i_rData, sizeof(double) * i_rNumber * RAY_PAR);
	*/

    double ori[3], dir[3], p1[3], p2[3], p3[3];

    int tri, ray;
    rayLoop: for(ray = 0; ray < i_rNumber; ray++)
    {
		#pragma HLS LOOP_TRIPCOUNT min=40000 max=40000

        int rayBase = ray * rayAttributeNumber;

        o_tIntersects[ray] = infinity; /// minimum t equals infinity

        /// getting ray data
        ori[0] = i_rData[rayBase],
		ori[1] = i_rData[rayBase + 1],
		ori[2] = i_rData[rayBase + 2];

        dir[0] = i_rData[rayBase + 3],
		dir[1] = i_rData[rayBase + 4],
		dir[2] = i_rData[rayBase + 5];


        triLoop: for(tri = 0; tri < i_tNumber; tri++)
        {
			#pragma HLS LOOP_TRIPCOUNT min=2000 max=2000

            int triBase = tri * triangleAttributeNumber;

            p1[0] = i_tData[triBase],
		    p1[1] = i_tData[triBase + 1],
		    p1[2] = i_tData[triBase + 2];

            p2[0] = i_tData[triBase + 3],
		    p2[1] = i_tData[triBase + 4],
			p2[2] = i_tData[triBase + 5];

            p3[0] = i_tData[triBase + 6],
            p3[1] = i_tData[triBase + 7],
			p3[2] = i_tData[triBase + 8];

            double a = p1[0] - p2[0],
            	   b = p1[0] - p3[0],
                   c = dir[0],
				   d = p1[0] - ori[0];

            double e = p1[1] - p2[1],
            	   f = p1[1] - p3[1],
                   g = dir[1],
				   h = p1[1] - ori[1];

            double i = p1[2] - p2[2],
            	   j = p1[2] - p3[2],
                   k = dir[2],
				   l = p1[2] - ori[2];

            double m = f * k - g * j,
            	   n = h * k - g * l,
				   p = f * l - h * j;

            double q = g * i - e * k,
            	   s = e * j - f * i;

            double inv_denom = 1.0 / (a * m + b * q + c * s);

            double e1 = d*m - b*n - c*p;
            double beta = e1*inv_denom;

            if(beta >= 0.0)
            {

				double r = e*l - h*i;
				double e2 = a*n + d*q + c*r;
				double gamma = e2 * inv_denom;

				if(gamma >= 0.0)
				{
					if(beta + gamma <= 1.0)
					{
						double e3 = a*p - b*r + d*s;
						double tmin = e3 * inv_denom;

						if(tmin >= EPSILON)
						{
							if(tmin < o_tIntersects[ray]) /// if the intersection point is the closest yet
							{
								o_tIntersects[ray] = tmin,  /// sets the minimum t value of the ray
								o_tIds[ray] = i_tIds[tri];  /// and sets the id of the closest triangle
							}
						}
					}
				}
            }

        }

        if(o_tIntersects[ray] >= (infinity - EPSILON)) /// if no triangle was hit
        {
            /// sets output id of the ray to -1
            /// meaning that no triangle will be shaded
            o_tIds[ray] = -1;

        }
    }
}



/*
 * #include "main.h"


 #define MAX_TRIS 2000
 #define MAX_RAYS 40000
 #define TRI_PAR 9
 #define RAY_PAR 6


//double local_tData[MAX_TRIS * TRI_PAR];

void intersectFPGA(
	int i_tNumber,          /// number of triangles
	volatile double *i_tData,        /// triangle information.    Size = i_tNumber * Triangle attribute number(12)
	volatile int *i_tIds,            /// triangle id information. Size = i_tNumber
	int i_rNumber,          /// number of rays
	volatile double *i_rData,        /// ray information                             Size = i_rNumber * Ray Attr number(6)
	volatile int *o_tIds,            /// min intersection triangle ids for each ray. Size = i_rNumber
	volatile double *o_tIntersects   /// min intersection t value for each ray       Size = i_rNumber
	)
{

//#pragma HLS ARRAY_PARTITION variable=local_tData block factor=100 dim=1
#pragma HLS INTERFACE s_axilite port=return bundle=control

#pragma HLS INTERFACE s_axilite port=i_tData bundle=control
#pragma HLS INTERFACE m_axi depth=600000 port=i_tData //bundle=data

#pragma HLS INTERFACE s_axilite port=i_tIds bundle=control
#pragma HLS INTERFACE m_axi depth=50000 port=i_tIds //bundle=data

#pragma HLS INTERFACE s_axilite port=i_rData bundle=control
#pragma HLS INTERFACE m_axi depth=12441600 port=i_rData //bundle=data

#pragma HLS INTERFACE s_axilite port=o_tIds bundle=control
#pragma HLS INTERFACE m_axi depth=2073600 port=o_tIds //bundle=data

#pragma HLS INTERFACE s_axilite port=o_tIntersects bundle=control
#pragma HLS INTERFACE m_axi depth=2073600 port=o_tIntersects // bundle=data

#pragma HLS INTERFACE s_axilite port=i_tNumber bundle=control
#pragma HLS INTERFACE s_axilite port=i_rNumber bundle=control

	const int triangleAttributeNumber = 9; /// p1, p2, p3 and normal * 3
    const int rayAttributeNumber = 6;       /// origin and direction  * 3
    const double EPSILON = 0.001;
    const double infinity = 1000000000.0;

    //memcpy(local_tData, i_tData, i_tNumber*TRI_PAR);


    double tris[MAX_TRIS * TRI_PAR], rays[MAX_RAYS * RAY_PAR];
    double t_p1[3], t_p2[3], t_p3[3], t_normal[3];
    double r_origin[3], r_direction[3];

    memcpy(tris, i_tData, sizeof(double) * i_tNumber * TRI_PAR);

    int tri, ray;
    rayLoop: for(ray = 0; ray < i_rNumber; ray++)
    {
		#pragma HLS LOOP_TRIPCOUNT min=40000 max=2073600 avg=480000

        int rayBase = ray * rayAttributeNumber;

        int hitAny = 0; /// for checking if a ray hit any triangle

        o_tIntersects[ray] = infinity; /// minimum t equals infinity

        /// getting ray data
        r_origin[0] = i_rData[rayBase];
        r_origin[1] = i_rData[rayBase + 1];
        r_origin[2] = i_rData[rayBase + 2];

        r_direction[0] = i_rData[rayBase + 3];
        r_direction[1] = i_rData[rayBase + 4];
        r_direction[2] = i_rData[rayBase + 5];


        triLoop: for(tri = 0; tri < i_tNumber; tri++)
        {
			#pragma HLS LOOP_TRIPCOUNT min=2000 max=50000 avg=10000

            int triBase = tri * triangleAttributeNumber;

            t_p1[0] = tris[triBase];
            t_p1[1] = tris[triBase + 1];
            t_p1[2] = tris[triBase + 2];

            t_p2[0] = tris[triBase + 3];
            t_p2[1] = tris[triBase + 4];
            t_p2[2] = tris[triBase + 5];

            t_p3[0] = tris[triBase + 6];
            t_p3[1] = tris[triBase + 7];
            t_p3[2] = tris[triBase + 8];

            double a = t_p1[0] - t_p2[0],
            	   b = t_p1[0] - t_p3[0],
                   c = r_direction[0],
				   d = t_p1[0] - r_origin[0];

            double e = t_p1[1] - t_p2[1],
            	   f = t_p1[1] - t_p3[1],
                   g = r_direction[1],
				   h = t_p1[1] - r_origin[1];

            double i = t_p1[2] - t_p2[2],
            	   j = t_p1[2] - t_p3[2],
                   k = r_direction[2],
				   l = t_p1[2] - r_origin[2];

            double m = f * k - g * j,
            	   n = h * k - g * l,
				   p = f * l - h * j;

            double q = g * i - e * k,
            	   s = e * j - f * i;

            double inv_denom = 1.0/(a*m + b*q + c*s);

            double e1 = d*m - b*n - c*p;
            double beta = e1*inv_denom;

            if(beta >= 0.0)
            {

				double r = e*l - h*i;
				double e2 = a*n + d*q + c*r;
				double gamma = e2 * inv_denom;

				if(gamma >= 0.0)
				{
					if(beta + gamma <= 1.0)
					{
						double e3 = a*p - b*r + d*s;
						double tmin = e3 * inv_denom;

						if(tmin >= EPSILON)
						{
							if(tmin < o_tIntersects[ray]) /// if the intersection point is the closest yet
							{
								o_tIntersects[ray] = tmin,  /// sets the minimum t value of the ray
								o_tIds[ray] = i_tIds[tri];  /// and sets the id of the closest triangle
							}
						}
					}
				}
            }

        }

        if(o_tIntersects[ray] >= (infinity - EPSILON)) /// if no triangle was hit
        {
            /// sets output id of the ray to -1
            /// meaning that no triangle will be shaded
            o_tIds[ray] = -1;

        }
    }
}
 *
 * */
