/* file system */
extern bool spiffsActive;
#if	defined(USE_SPIFFS)
extern SPIFFSConfig FScfg;
#endif
#if	defined(USE_LITTLEFS)
extenr LittleFSConfig FScfg;
#endif
extern Dir fs_root, fs_rnode;
extern void flash_init(void);
extern void flash_gc(void);
extern void flash_dir(void);
