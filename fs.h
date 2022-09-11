#if	!defined(_SIOBRIDGE_fs_h)
#define	_SIOBRIDGE_fs_h

/* file system */
extern bool spiffsActive;
#if	defined(USE_SPIFFS)
extern SPIFFSConfig FScfg;
#endif
#if	defined(USE_LITTLEFS)
extenr LittleFSConfig FScfg;
#endif
extern Dir fs_root, fs_rnode;
extern void flash_setup(void);
extern void flash_gc(void);
extern void flash_dir(void);

#endif
