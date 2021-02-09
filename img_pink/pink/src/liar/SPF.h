
#ifdef __cplusplus
extern "C" {
#endif

struct xvimage * SPF_double(uint32_t ** edges,
			double * weights, 
			uint32_t *seeds, 
			uint8_t * labels,
			int size_seeds, 
			int rs,  /* taille ligne */
			int cs,   /* taille colonne */
			int ds   /* depth */
			);  
#ifdef __cplusplus
}
#endif
