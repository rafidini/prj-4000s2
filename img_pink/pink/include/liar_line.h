/*
 * header for the fseries
 *
 * Hugues Talbot	 3 Jun 2011
 *
 * These functions are the C <-> C++ bridge to access the f-series from the command line
 *
 */

#ifdef __cplusplus
extern "C" {
#endif

/* these are all pure C functions */
    int imopenbun( const struct xvimage *input, /**< [in] input image */
                   int radius,                  /**< [in] radius of line segments */
                   int n,                       /**< [in] number of lines to consider */
                   double angle,                /**< [in] starting (line) angle */
                   double range,                /**< [in] line angle range */
                   double rank,                 /**< [in] 0 <= rank <= 1 */
                   struct xvimage *output       /**< [out] output image */);

    int imclosebin( const struct xvimage *input, /**< [in] input image */
                    int radius,                  /**< [in] radius of line segments */
                    int n,                       /**< [in] number of lines to consider */
                    double angle,                /**< [in] starting (line) angle */
                    double range,                /**< [in] line angle range */
                    double rank,                 /**< [in] 0 <= rank <= 1 */
                    struct xvimage *output       /**< [out] output image */);

#ifdef __cplusplus
}
#endif
