// -----------------------------------------------------------------------------
// file

char *file_name(const char *pathfile) {
    char *s = strrchr(pathfile, '/'), *t = strrchr(pathfile, '\\');
    return va("%s", s > t ? s+1 : t ? t+1 : pathfile);
}
char *file_pathabs( const char *pathfile ) {
    char *out = va("%.*s", DIR_MAX+1, "");
#ifdef _WIN32
    _fullpath(out, pathfile, DIR_MAX);
#else
    realpath(pathfile, out);
#endif
    return out;
}
char *file_path(const char *pathfile) {
    return va("%.*s", (int)(strlen(pathfile)-strlen(file_name(pathfile))), pathfile);
}
char *file_load(const char *filename, int *len) { // @todo: fix leaks
    FILE *fp = filename[0] ? fopen(filename, "rb") : NULL;
    if( fp ) {
        fseek(fp, 0L, SEEK_END);
        size_t sz = ftell(fp);
        fseek(fp, 0L, SEEK_SET);
        char *bin = REALLOC(0, sz+1);
        fread(bin,sz,1,fp);
        fclose(fp);
        bin[sz] = 0;
        if(len) *len = (int)sz;
        return bin;
    }
    if (len) *len = 0;
    return 0;
}
char *file_read(const char *filename) { // @todo: fix leaks
    return file_load(filename, NULL);
}
bool file_move(const char *src, const char *dst) {
    bool ok = file_exist(src) && !file_exist(dst) && 0 == rename(src, dst);
    return ok;
}
bool file_delete(const char *pathfile) {
    if( file_exist(pathfile) ) {
        for( int i = 0; i < 10; ++i ) {
            bool ok = 0 == unlink(pathfile);
            if( ok ) return true;
            sleep_ms(10);
        }
        return false;
    }
    return true;
}
uint64_t file_stamp(const char *fname) {
    time_t mtime = (time_t)file_stamp_epoch(fname);
    struct tm *ti = localtime(&mtime);
    return atoi64(va("%04d%02d%02d%02d%02d%02d",ti->tm_year+1900,ti->tm_mon+1,ti->tm_mday,ti->tm_hour,ti->tm_min,ti->tm_sec));
}
static bool file_stat(const char *fname, struct stat *st) {
    // remove ending slashes. win32+tcc does not like them.
    int l = strlen(fname), m = l;
    while( l && (fname[l-1] == '/' || fname[l-1] == '\\') ) --l;
    fname = l == m ? fname : va("%.*s", l, fname);
    return stat(fname, st) >= 0;
}
uint64_t file_stamp_epoch(const char *fname) {
    struct stat st;
    return !file_stat(fname, &st) ? 0ULL : st.st_mtime;
}
uint64_t file_size(const char *fname) {
    struct stat st;
    return !file_stat(fname, &st) ? 0ULL : st.st_size;
}
bool file_directory( const char *pathfile ) {
    struct stat st;
    return !file_stat(pathfile, &st) ? 0 : S_IFDIR == ( st.st_mode & S_IFMT );
}
bool file_exist(const char *fname) {
    struct stat st;
    return !file_stat(fname, &st) ? false : true;
}
char *file_normalize(const char *name) {
    char *copy = va("%s", name), *s = copy, c;
#if is(win32)
    for( int i = 0; copy[i]; ++i ) { if(copy[i] == '/') copy[i] = '\\'; if(copy[i] == '\'') copy[i] = '\"'; }
#else
    for( int i = 0; copy[i]; ++i ) { if(copy[i] == '\\') copy[i] = '/'; if(copy[i] == '\"') copy[i] = '\''; }
#endif
    return copy;
}
#if 0
char *file_normalize(const char *name) {
    char *copy = va("%s", name), *s = copy, c;
    // lowercases+digits+underscores+slashes only. anything else is truncated.
    for( ; *name ; ++name ) {
        /**/ if( *name >= 'a' && *name <= 'z' ) *s++ = *name;
        else if( *name >= 'A' && *name <= 'Z' ) *s++ = *name - 'A' + 'a';
        else if( *name >= '0' && *name <= '9' ) *s++ = *name;
        else if( *name == '/' || *name == '\\') *s++ = '/';
        else if( *name <= ' ' || *name == '.' ) *s++ = '_';
    } *s++ = 0;
    // remove dupe slashes
    for( name = s = copy, c = '/'; *name ; ) {
        while( *name && *name != c ) *s++ = *name++;
        if( *name ) *s++ = c;
        while( *name && *name == c ) name++;
    } *s++ = 0;
    // remove dupe underlines
    for( name = s = copy, c = '_'; *name ; ) {
        while( *name && *name != c ) *s++ = *name++;
        if( *name ) *s++ = c;
        while( *name && *name == c ) name++;
    } *s++ = 0;
    return copy;
}
char *file_normalize_with_folder(const char *name) {
    char *s = file_normalize(name);
    char *slash = strrchr(s, '/'); if(slash) *slash = 0;
    char *penultimate = strrchr(s, '/'); if(slash) *slash = '/';
    return penultimate ? penultimate+1 : /*slash ? slash+1 :*/ s;
}
#endif
char *file_ext(const char *name) {
    char *b = file_name(name), *s = strchr(b, '.'); //strrchr(name, '.');
    return va("%s", s ? s : "" ); // , name );
}

char *file_id(const char *pathfile) {
    char *dir = file_path(pathfile); for(int i=0;dir[i];++i) dir[i]=tolower(dir[i]);
    char *base = file_name(pathfile); for(int i=0;base[i];++i) base[i]=tolower(base[i]);
#if 0 // extensionless, larry.mid and larry.txt will collide, diffuse.tga and diffuse.png will match
char *ext = strchr(base, '.'); if (ext) ext[0] = '\0'; // remove all extensions
#else // extensionless for audio/images only (materials: diffuse.tga and diffuse.png will match)
char *ext = strrchr(base, '.'); //if (ext) ext[0] = '\0'; // remove all extensions
    if(ext) if( strstr(".jpg.png.bmp.tga.hdr"".", ext) || strstr(".ogg.mp3.wav.mod.xm.flac"".", ext) || strstr(".mp4.ogv.avi.mkv.wmv.mpg.mpeg"".", ext) ) {
        ext = strchr(base, '.');
        ext[0] = '\0'; //strcpy(ext, "_xxx");
    }
#endif

// if (!dir[0]) return base;
    char *stem = va("%s/%s", dir, base); // file_name(dir);

    // /path2/path1/file2_file1 -> file1_file2/path1/path2
    int len = 0;
    int ids_count = 0;
    char ids[64][64] = { 0 };
    // split path stems
    for each_substring(stem, "/\\", key) {
        int tokens_count = 0;
        char* tokens[64] = { 0 };
        // split tokens
        for each_substring(key, "[]()_ ", it) {
            tokens[tokens_count++] = va("%s", it);
        }
        // sort alphabetically
        if( tokens_count > 1 ) qsort(tokens, tokens_count, sizeof(char *), strcmp_qsort);
        // concat sorted token1_token2_...
        char built[256]; *built = 0;
        for( int i = 0; i < tokens_count; ++i ) {
            strlcat(built, "_", 256);
            strlcat(built, tokens[i], 256);
        }
        strncpy( ids[ ids_count ], &built[1], 64 );
        len += strlen( ids[ ids_count++ ] );
    }
    // concat in inverse order: file/path1/path2/...
    char buffer[512]; buffer[0] = 0;
    for( int it = ids_count; --it >= 0; ) {
        strcat(buffer, ids[it]);
        strcat(buffer, "/");
    }
    return va("%s", buffer);
}
const char** file_list(const char *cwd, const char *masks) {
    ASSERT(strendi(cwd, "/"), "Error: dirs like '%s' must end with slash", cwd);

    static threadlocal array(char*) list = 0;
    const char *arg0 = cwd; // app_path();
    int larg0 = strlen(arg0);

    for( int i = 0; i < array_count(list); ++i ) {
        FREE(list[i]);
    }
    array_free(list);

    for each_substring(masks,";",it) {
        int recurse = !!strstr(it, "**");
        #if is(win32)
        char *glob = va("dir %s/b/o:n \"%s\\%s\" 2> NUL", recurse ? "/s":"", cwd, it);
        #else // linux, osx
        char *glob = va("find %s -type f -iname \"%s\" | sort", cwd, it); // @fixme: add non-recursive support
        #endif
        for( FILE *in = popen(glob, "r"); in; pclose(in), in = 0) {
            char buf[1024], *line = buf;
            while( fgets(buf, sizeof(buf), in) ) {
                // clean up
                if( strstr(line, arg0) ) line = buf + larg0;
                if( !memcmp(line, "./", 2) ) line += 2;
                int len = strlen(line); while( len > 0 && line[len-1] < 32 ) line[--len] = 0;
                for(int i = 0; i < len; ++i ) if(line[i] == '\\') line[i] = '/';
                // do not insert system folders/files
                if( strstr(line, "/.") ) continue;
                // insert copy
                #if is(win32)
                char *copy = STRDUP(line); // full path already provided
                #else
                while(line[0] == '/') ++line;
                char *copy = STRDUP(va("%s%s", cwd, line)); // need to prepend path
                #endif
                array_push(list, copy);
            }
        }
    }
    array_push(list, 0); // terminator
    return (const char**)list;
}

bool file_copy(const char *src, const char *dst) {
    int ok = 0, BUFSIZE = 1 << 20; // 1 MiB
    static threadlocal char *buffer = 0; do_once buffer = REALLOC(0, BUFSIZE);
    for( FILE *in = fopen(src, "rb"); in; fclose(in), in = 0) {
        for( FILE *out = fopen(dst, "wb"); out; fclose(out), out = 0, ok = 1) {
            for( int n; !!(n = fread( buffer, 1, BUFSIZE, in )); ){
                if(fwrite( buffer, 1, n, out ) != n)
                return fclose(in), fclose(out), false;
            }
        }
    }
    return ok;
}

char* file_tempname() {
    static __thread int id;
    return va("%s/fwk-temp.%s.%p.%d", app_temp(), getenv(ifdef(win32, "username", "USER")), &id, rand());
}

FILE *file_temp(void) {
    const char *fname = file_tempname();
    FILE *fp = fopen(fname, "w+b");
    if( fp ) unlink(fname);
    return fp;
}

// -----------------------------------------------------------------------------
// archives

enum { is_zip, is_tar, is_pak, is_dir };

typedef struct archive_dir {
    char* path;
    union {
        int type;
        int size; // for cache only
    };
    union {
        void *archive;
        void *data; // for cache only
        zip* zip_archive;
        tar* tar_archive;
        pak* pak_archive;
    };
    struct archive_dir *next;
} archive_dir;

static archive_dir *dir_mount;
static archive_dir *dir_cache; enum { MAX_CACHED_FILES = 16 }; // @todo: should we cache the cooked contents instead? ie, stbi() result instead of file.png?

struct vfs_entry {
    const char *name;
    const char *id;
    unsigned size;
};
array(struct vfs_entry) vfs_entries;

bool vfs_mount(const char *path) {
    zip *z = NULL; tar *t = NULL; pak *p = NULL; dir *d = NULL;
    int is_folder = ('/' == path[strlen(path)-1]);
    if( is_folder ) d = dir_open(path, "rb");
    if( is_folder && !d ) return 0;
    if( !is_folder ) z = zip_open(path, "rb");
    if( !is_folder && !z ) t = tar_open(path, "rb");
    if( !is_folder && !z && !t ) p = pak_open(path, "rb");
    if( !is_folder && !z && !t && !p ) return 0;

    // normalize input -> "././" to ""
    while (path[0] == '.' && path[1] == '/') path += 2;
    path = STRDUP(path);
    if( z || t || p ) {
    // save local path for archives, so we can subtract those from upcoming requests
    if(strrchr(path,'/')) strrchr(path,'/')[1] = '\0';
    } else if(d) 0[(char*)path] = 0;

    // append to mounted archives
    archive_dir *prev = dir_mount, zero = {0};
    *(dir_mount = REALLOC(0, sizeof(archive_dir))) = zero;
    dir_mount->next = prev;
    dir_mount->path = (char*)path;
    dir_mount->archive = z ? (void*)z : t ? (void*)t : p ? (void*)p : (void*)d;
    dir_mount->type = is_folder ? is_dir : z ? is_zip : t ? is_tar : p ? is_pak : -1;
    ASSERT(dir_mount->type >= 0 && dir_mount->type < 4);

    // append list of files to internal listing
    for( archive_dir *dir = dir_mount; dir ; dir = 0 ) { // for(archive_dir *dir = dir_mount; dir; dir = dir->next) {
        assert(dir->type >= 0 && dir->type < 4);
        unsigned (*fn_count[4])(void*) = {(void*)zip_count, (void*)tar_count, (void*)pak_count, (void*)dir_count};
        char*    (*fn_name[4])(void*, unsigned index) = {(void*)zip_name, (void*)tar_name, (void*)pak_name, (void*)dir_name};
        unsigned (*fn_size[4])(void*, unsigned index) = {(void*)zip_size, (void*)tar_size, (void*)pak_size, (void*)dir_size};

        for( unsigned idx = 0, end = fn_count[dir->type](dir->archive); idx < end; ++idx ) {
            assert(idx < end);
            const char *filename = STRDUP( fn_name[dir->type](dir->archive, idx) );
            const char *fileid = STRDUP( file_id(filename) );
            unsigned filesize = fn_size[dir->type](dir->archive, idx);
            // printf("%u) %s %u [%s]\n", idx, filename, filesize, fileid);
            // append to list
            array_push(vfs_entries, (struct vfs_entry){filename, fileid, filesize});
        }
    }

    return 1;
}
const char** vfs_list(const char *masks) {
    static threadlocal array(char*) list = 0;

    for( int i = 0; i < array_count(list); ++i ) {
        FREE(list[i]);
    }
    array_free(list);

    for each_substring(masks,";",it) {
        it = va("*/%s", it);
        // int recurse = !!strstr(it, "**"); // @fixme: support non-recursive
        for( unsigned i = 0; i < array_count(vfs_entries); ++i ) {
            const char *name = vfs_entries[i].name;
            if( strmatch(name, it) ) {
                // insert copy
                char *copy = STRDUP(name);
                array_push(list, copy);
            }
        }
    }

    // sort alphabetically
    qsort(list, array_count(list), sizeof(char *), strcmp_qsort);

    array_push(list, 0); // terminator
    return (const char**)list;
}

static
char *vfs_unpack(const char *pathfile, int *size) { // must free() after use
    // @todo: add cache here
    char *data = NULL;
    for(archive_dir *dir = dir_mount; dir && !data; dir = dir->next) {
        if( dir->type == is_dir ) {
#if 1 // sandboxed
            char buf[512];
            snprintf(buf, sizeof(buf), "%s%s", dir->path, pathfile);
            data = file_load(buf, size);
#endif
        } else {
            int      (*fn_find[3])(void *, const char *) = {(void*)zip_find, (void*)tar_find, (void*)pak_find};
            void*    (*fn_unpack[3])(void *, unsigned) = {(void*)zip_extract, (void*)tar_extract, (void*)pak_extract};
            unsigned (*fn_size[3])(void *, unsigned) = {(void*)zip_size, (void*)tar_size, (void*)pak_size};

#if 0
            const char* cleanup = pathfile + strbegi(pathfile, dir->path) * strlen(dir->path);
            while (cleanup[0] == '/') ++cleanup;
#else
            const char *cleanup = pathfile;
#endif          

            int index = fn_find[dir->type](dir->archive, cleanup);
            data = fn_unpack[dir->type](dir->archive, index);
            if( size ) *size = fn_size[dir->type](dir->archive, index);
        }
        // printf("%c trying %s in %s ...\n", data ? 'Y':'N', pathfile, dir->path);
    }
    //wait_ms(1000); // <-- simulate slow hdd
    return data;
}

const char *vfs_resolve(const char *pathfile) {
    // we dont resolve absolute paths. they dont belong to the vfs
    if( pathfile[0] == '/' || pathfile[0] == '\\' || pathfile[1] == ':' ) return pathfile;

    // find best match
    char* id = file_id(pathfile);
    for (int i = array_count(vfs_entries); --i >= 0; ) {
        if (strbegi(vfs_entries[i].id, id) ) {
            return vfs_entries[i].name;
        }
    }

    return pathfile;
}

char* vfs_load(const char *pathfile, int *size_out) { // @todo: fix leaks
    if (!pathfile[0]) return file_load(pathfile, size_out);
    if (pathfile[0] == '/' || pathfile[1] == ':') return file_load(pathfile, size_out);

    //{
    // exclude garbage from material names
    // @todo: exclude double slashs in paths
    char *base = file_name(pathfile); if(strchr(base,'+')) base = strchr(base, '+')+1;
    char *folder = file_path(pathfile);
    pathfile = va("%s%s", folder, base);

    // solve virtual path
    pathfile = va("%s", vfs_resolve(pathfile));
    base = file_name(pathfile);
    folder = file_path(pathfile);
        // make folder variable easier to read in logs: /home/rlyeh/prj/fwk/art/demos/audio/coin.wav -> demos/audio/coin.wav 
        static int artlen = 0; do_once artlen = strlen(ART);
        if( !strncmp(folder, ART, artlen) ) {
            folder += artlen;
        }
    //}

    int size = 0;
    void *ptr = 0;

#if 0
    // clean pathfile
    while (pathfile[0] == '.' && pathfile[1] == '/') pathfile += 2;
    while (pathfile[0] == '/') ++pathfile;
#endif

    const char *lookup_id = /*file_normalize_with_folder*/(pathfile);

    // search (last item)
    static char last_item[256] = { 0 };
    static void *last_ptr = 0;
    static int   last_size = 0;
    if( !strcmpi(lookup_id, last_item)) {
        ptr = last_ptr;
        size = last_size;
    }

    // search (cache)
    if( !ptr ) {
        ptr = cache_lookup(lookup_id, &size);
    }

    if( ptr ) {
        PRINTF("Loading VFS (%s)%s (cached)\n", folder, base);
    } else {
        PRINTF("Loading VFS (%s)%s\n", folder, base);
    }

    // search (mounted disks)
    if( !ptr ) {
        ptr = vfs_unpack(pathfile, &size);
        if( ptr ) {
            cache_insert(lookup_id, ptr, size);
        }
    }

    if( ptr && size )
    if( ptr != last_ptr) {
        snprintf(last_item, 256, "%s", lookup_id);
        last_ptr = ptr;
        last_size = size;
    }

    if(!ptr) {
        PRINTF("Loading %s (not found)\n", pathfile);
    }

    if( size_out ) *size_out = ptr ? size : 0;
    return ptr;
}
char* vfs_read(const char *pathfile) {
    return vfs_load(pathfile, NULL);
}
int vfs_size(const char *pathfile) {
    int sz;
    return vfs_load(pathfile, &sz), sz;
}


FILE* vfs_handle(const char *pathfile) { // preferred way, will clean descriptors at exit
    int sz;
    char *buf = vfs_load(pathfile, &sz);
    FILE *fp = fmemopen(buf ? buf : "", buf ? sz : 0, "a+b");
    ASSERT( fp, "cannot create tempfile" );
    return fp;
}
const char *vfs_find(const char *pathfile) {
#if 0
    FILE* fp = vfs_handle(pathfile);
    return fp ? pathfile_from_handle(fp) : "";
#else
    int sz;
    char *buf = vfs_load(pathfile, &sz);
    if( !buf ) return "";

    // pool of temp files. recycles after every loop
    enum { MAX_TEMP_FILES = 16 };
    static threadlocal char temps[MAX_TEMP_FILES][DIR_MAX] = {0};
    static threadlocal int i = 0;
    if( temps[i]) unlink(temps[i]);
    i = (i + 1) % MAX_TEMP_FILES;
    if(!temps[i][0]) snprintf(temps[i], DIR_MAX, "%s", tmpnam(0));
    char *name = temps[i];

    FILE *tmp = fopen(name, "wb"); //unlink(name);
    ASSERT( tmp, "cannot create tempfile %s", name );
    fwrite(buf ? buf : "", 1, buf ? sz : 0, tmp);
    fclose(tmp);
    return name;
#endif
}


// -----------------------------------------------------------------------------
// cache

void* cache_lookup(const char *pathfile, int *size) { // find key->value
    if( !MAX_CACHED_FILES ) return 0;
    for(archive_dir *dir = dir_cache; dir; dir = dir->next) {
        if( !strcmp(dir->path, pathfile) ) {
            if(size) *size = dir->size;
            return dir->data;
        }
    }
    return 0;
}
void* cache_insert(const char *pathfile, void *ptr, int size) { // append key/value; return LRU or NULL
    if( !MAX_CACHED_FILES ) return 0;
    assert( ptr );
    assert( size );

    // append to cache
    archive_dir zero = {0}, *old = dir_cache;
    *(dir_cache = REALLOC(0, sizeof(archive_dir))) = zero;
    dir_cache->next = old;
    dir_cache->path = STRDUP(pathfile);
    dir_cache->size = size;
    dir_cache->data = REALLOC(0, size+1);
    memcpy(dir_cache->data, ptr, size); size[(char*)dir_cache->data] = 0; // copy+terminator

    // keep cached files within limits
    static int added = 0;
    if( added < MAX_CACHED_FILES ) {
        ++added;
    } else {
        // remove oldest cache entry
        for( archive_dir *prev = dir_cache, *dir = prev; dir ; prev = dir, dir = dir->next ) {
            if( !dir->next ) {
                prev->next = 0; // break link
                void *data = dir->data;
                dir->path = REALLOC(dir->path, 0);
                dir->data = REALLOC(dir->data, 0);
                dir = REALLOC(dir, 0);
                return data;
            }
        }
    }
    return 0;
}
