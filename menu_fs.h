
static const char *menu_fs_help[] = {
  "**************\r\n",
  "* Filesystem *\r\n",
  "**************\r\n",
  "\r\n",
  "D\tDownload from fs\r\n",
  "L\tList fs\r\n",
  "U\tUpload to fs\r\n",
  "\r\n",
  "F\tFormat Flashfs\r\n",
  "Q\tBack to main menu\r\n",
  NULL
};
MenuItem menu_fs[] = {
  { 'D', cmd_fs_download, NULL },
  { 'L', cmd_fs_list, NULL },
  { 'U', cmd_fs_upload, NULL },
  { 'Q', NULL, menu_main },
  {   0, NULL, NULL }
};
