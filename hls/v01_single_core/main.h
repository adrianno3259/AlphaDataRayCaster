void intersectFPGA(
                int i_tNumber,          /// number of triangles
                volatile double *i_tData,        /// triangle information.    Size = i_tNumber * Triangle attribute number(12)
                volatile int *i_tIds,            /// triangle id information. Size = i_tNumber
                int i_rNumber,          /// number of rays
                volatile double *i_rData,        /// ray information                             Size = i_rNumber * Ray Attr number(6)
                volatile int *o_tIds,            /// min intersection triangle ids for each ray. Size = i_rNumber
                volatile double *o_tIntersects   /// min intersection t value for each ray       Size = i_rNumber
                );
