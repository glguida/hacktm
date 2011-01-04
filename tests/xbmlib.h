#define LOAD_XBM(str, name)			\
  do {						\
    (str)->width = name##_width;		\
    (str)->height = name##_height;		\
    (str)->bits = name##_bits;			\
  } while ( 0 )


namespace xbmlib {

  struct xbmImage {
    unsigned width;
    unsigned height;
    unsigned char *bits;
  };

  void setBitVectorFromXbm(xbmImage &i, hacktm::BitVector &bv);
}
