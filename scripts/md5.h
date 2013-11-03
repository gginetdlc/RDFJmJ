void MD5(char[] , char[] );

typedef union uwb {
    unsigned w;
    unsigned char b[4];
} WBunion;
 
typedef unsigned Digest[4];
unsigned f0( unsigned [] );
unsigned f1( unsigned [] );
unsigned f2( unsigned [] );
unsigned f3( unsigned [] );
typedef unsigned (*DgstFctn)(unsigned []);

unsigned *md5( const char *, int );
unsigned rol( unsigned , short  );
unsigned *calcKs( unsigned *);

