#ifndef DIRENT_H
#define DIRENT_H

#if defined(__clang__)
#	pragma clang diagnostic ignored "-Wunused-function"
#elif defined(_MSC_VER)
#	pragma warning(disable:4505)
#elif defined(__GNUC__)
#	pragma GCC diagnostic ignored "-Wunused-function"
#endif

#ifndef WIN32_LEAN_AND_MEAN
#	define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

#include <stdio.h>
#include <stdarg.h>
#include <wchar.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>

#define _DIRENT_HAVE_D_TYPE

#define _DIRENT_HAVE_D_NAMLEN

#if !defined(FILE_ATTRIBUTE_DEVICE)
#	define FILE_ATTRIBUTE_DEVICE 0x40
#endif

#if !defined(S_IFMT)
#	define S_IFMT _S_IFMT
#endif

#if !defined(S_IFDIR)
#	define S_IFDIR _S_IFDIR
#endif

#if !defined(S_IFCHR)
#	define S_IFCHR _S_IFCHR
#endif

#if !defined(S_IFFIFO)
#	define S_IFFIFO _S_IFFIFO
#endif

#if !defined(S_IFREG)
#	define S_IFREG _S_IFREG
#endif

#if !defined(S_IREAD)
#	define S_IREAD _S_IREAD
#endif

#if !defined(S_IWRITE)
#	define S_IWRITE _S_IWRITE
#endif

#if !defined(S_IEXEC)
#	define S_IEXEC _S_IEXEC
#endif

#if !defined(S_IFIFO)
#	define S_IFIFO _S_IFIFO
#endif

#if !defined(S_IFBLK)
#	define S_IFBLK 0
#endif

#if !defined(S_IFLNK)
#	define S_IFLNK 0
#endif

#if !defined(S_IFSOCK)
#	define S_IFSOCK 0
#endif

#if !defined(S_IRUSR)
#	define S_IRUSR S_IREAD
#endif

#if !defined(S_IWUSR)
#	define S_IWUSR S_IWRITE
#endif

#if !defined(S_IXUSR)
#	define S_IXUSR 0
#endif

#if !defined(S_IRGRP)
#	define S_IRGRP 0
#endif

#if !defined(S_IWGRP)
#	define S_IWGRP 0
#endif

#if !defined(S_IXGRP)
#	define S_IXGRP 0
#endif

#if !defined(S_IROTH)
#	define S_IROTH 0
#endif

#if !defined(S_IWOTH)
#	define S_IWOTH 0
#endif

#if !defined(S_IXOTH)
#	define S_IXOTH 0
#endif

#if !defined(PATH_MAX)
#	define PATH_MAX MAX_PATH
#endif
#if !defined(FILENAME_MAX)
#	define FILENAME_MAX MAX_PATH
#endif
#if !defined(NAME_MAX)
#	define NAME_MAX FILENAME_MAX
#endif

#define DT_UNKNOWN 0
#define DT_REG S_IFREG
#define DT_DIR S_IFDIR
#define DT_FIFO S_IFIFO
#define DT_SOCK S_IFSOCK
#define DT_CHR S_IFCHR
#define DT_BLK S_IFBLK
#define DT_LNK S_IFLNK

#define IFTODT(mode) ((mode) & S_IFMT)
#define DTTOIF(type) (type)

#if !defined(S_ISFIFO)
#	define S_ISFIFO(mode) (((mode) & S_IFMT) == S_IFIFO)
#endif
#if !defined(S_ISDIR)
#	define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)
#endif
#if !defined(S_ISREG)
#	define S_ISREG(mode) (((mode) & S_IFMT) == S_IFREG)
#endif
#if !defined(S_ISLNK)
#	define S_ISLNK(mode) (((mode) & S_IFMT) == S_IFLNK)
#endif
#if !defined(S_ISSOCK)
#	define S_ISSOCK(mode) (((mode) & S_IFMT) == S_IFSOCK)
#endif
#if !defined(S_ISCHR)
#	define S_ISCHR(mode) (((mode) & S_IFMT) == S_IFCHR)
#endif
#if !defined(S_ISBLK)
#	define S_ISBLK(mode) (((mode) & S_IFMT) == S_IFBLK)
#endif

#define _D_EXACT_NAMLEN(p) ((p)->d_namlen)

#define _D_ALLOC_NAMLEN(p) ((PATH_MAX)+1)

#ifdef __cplusplus
extern "C" {
#endif

	struct _wdirent {
		long d_ino;

		long d_off;

		unsigned short d_reclen;

		size_t d_namlen;

		int d_type;

		wchar_t d_name[PATH_MAX + 1];
	};
	typedef struct _wdirent _wdirent;

	struct _WDIR {
		struct _wdirent ent;

		WIN32_FIND_DATAW data;

		int cached;

		HANDLE handle;

		wchar_t* patt;
	};
	typedef struct _WDIR _WDIR;

	struct dirent {
		long d_ino;

		long d_off;

		unsigned short d_reclen;

		size_t d_namlen;

		int d_type;

		char d_name[PATH_MAX + 1];
	};
	typedef struct dirent dirent;

	struct DIR {
		struct dirent ent;
		struct _WDIR* wdirp;
	};
	typedef struct DIR DIR;

	static DIR* opendir(const char* dirname);
	static _WDIR* _wopendir(const wchar_t* dirname);

	static struct dirent* readdir(DIR* dirp);
	static struct _wdirent* _wreaddir(_WDIR* dirp);

	static int readdir_r(
		DIR* dirp, struct dirent* entry, struct dirent** result);
	static int _wreaddir_r(
		_WDIR* dirp, struct _wdirent* entry, struct _wdirent** result);

	static int closedir(DIR* dirp);
	static int _wclosedir(_WDIR* dirp);

	static void rewinddir(DIR* dirp);
	static void _wrewinddir(_WDIR* dirp);

	static int scandir(const char* dirname, struct dirent*** namelist,
		int (*filter)(const struct dirent*),
		int (*compare)(const struct dirent**, const struct dirent**));

	static int alphasort(const struct dirent** a, const struct dirent** b);

	static int versionsort(const struct dirent** a, const struct dirent** b);

	static int strverscmp(const char* a, const char* b);

#define wdirent _wdirent
#define WDIR _WDIR
#define wopendir _wopendir
#define wreaddir _wreaddir
#define wclosedir _wclosedir
#define wrewinddir _wrewinddir

#if !defined(_MSC_VER) || _MSC_VER < 1400
#	define wcstombs_s dirent_wcstombs_s
#	define mbstowcs_s dirent_mbstowcs_s
#endif

#if defined(_MSC_VER) && _MSC_VER >= 1400
#	define dirent_set_errno _set_errno
#endif

	static WIN32_FIND_DATAW* dirent_first(_WDIR* dirp);
	static WIN32_FIND_DATAW* dirent_next(_WDIR* dirp);

#if !defined(_MSC_VER) || _MSC_VER < 1400
	static int dirent_mbstowcs_s(
		size_t* pReturnValue, wchar_t* wcstr, size_t sizeInWords,
		const char* mbstr, size_t count);
#endif

#if !defined(_MSC_VER) || _MSC_VER < 1400
	static int dirent_wcstombs_s(
		size_t* pReturnValue, char* mbstr, size_t sizeInBytes,
		const wchar_t* wcstr, size_t count);
#endif

#if !defined(_MSC_VER) || _MSC_VER < 1400
	static void dirent_set_errno(int error);
#endif

	static _WDIR* _wopendir(const wchar_t* dirname)
	{
		wchar_t* p;

		if (dirname == NULL || dirname[0] == '\0') {
			dirent_set_errno(ENOENT);
			return NULL;
		}

		_WDIR* dirp = (_WDIR*)malloc(sizeof(struct _WDIR));
		if (!dirp)
			return NULL;

		dirp->handle = INVALID_HANDLE_VALUE;
		dirp->patt = NULL;
		dirp->cached = 0;

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
		DWORD n = GetFullPathNameW(dirname, 0, NULL, NULL);
#else
		size_t n = wcslen(dirname);
#endif

		dirp->patt = (wchar_t*)malloc(sizeof(wchar_t) * n + 16);
		if (dirp->patt == NULL)
			goto exit_closedir;

#if WINAPI_FAMILY_PARTITION(WINAPI_PARTITION_DESKTOP)
		n = GetFullPathNameW(dirname, n, dirp->patt, NULL);
		if (n <= 0)
			goto exit_closedir;
#else
		wcsncpy_s(dirp->patt, n + 1, dirname, n);
#endif

		p = dirp->patt + n;
		switch (p[-1]) {
		case '\\':
		case '/':
		case ':':
			;
			break;

		default:
			*p++ = '\\';
		}
		*p++ = '*';
		*p = '\0';

		if (!dirent_first(dirp))
			goto exit_closedir;

		return dirp;

	exit_closedir:
		_wclosedir(dirp);
		return NULL;
	}

	static struct _wdirent* _wreaddir(_WDIR* dirp)
	{
		struct _wdirent* entry;
		(void)_wreaddir_r(dirp, &dirp->ent, &entry);

		return entry;
	}

	static int _wreaddir_r(
		_WDIR* dirp, struct _wdirent* entry, struct _wdirent** result)
	{
		WIN32_FIND_DATAW* datap = dirent_next(dirp);
		if (!datap) {
			*result = NULL;
			return 0;
		}

		size_t n = 0;
		while (n < PATH_MAX && datap->cFileName[n] != 0) {
			entry->d_name[n] = datap->cFileName[n];
			n++;
		}
		entry->d_name[n] = 0;

		entry->d_namlen = n;

		DWORD attr = datap->dwFileAttributes;
		if ((attr & FILE_ATTRIBUTE_DEVICE) != 0)
			entry->d_type = DT_CHR;
		else if ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
			entry->d_type = DT_DIR;
		else
			entry->d_type = DT_REG;

		entry->d_ino = 0;
		entry->d_off = 0;
		entry->d_reclen = sizeof(struct _wdirent);

		*result = entry;
		return 0;
	}

	static int _wclosedir(_WDIR* dirp)
	{
		if (!dirp) {
			dirent_set_errno(EBADF);
			return -1;
		}

		if (dirp->handle != INVALID_HANDLE_VALUE)
			FindClose(dirp->handle);

		free(dirp->patt);

		free(dirp);
		return 0;
	}

	static void _wrewinddir(_WDIR* dirp)
	{
		if (!dirp)
			return;

		if (dirp->handle != INVALID_HANDLE_VALUE)
			FindClose(dirp->handle);

		dirent_first(dirp);
	}

	static WIN32_FIND_DATAW* dirent_first(_WDIR* dirp)
	{
		if (!dirp)
			return NULL;

		dirp->handle = FindFirstFileExW(
			dirp->patt, FindExInfoStandard, &dirp->data,
			FindExSearchNameMatch, NULL, 0);
		if (dirp->handle == INVALID_HANDLE_VALUE)
			goto error;

		dirp->cached = 1;
		return &dirp->data;

	error:
		dirp->cached = 0;

		DWORD errorcode = GetLastError();
		switch (errorcode) {
		case ERROR_ACCESS_DENIED:
			dirent_set_errno(EACCES);
			break;

		case ERROR_DIRECTORY:
			dirent_set_errno(ENOTDIR);
			break;

		case ERROR_PATH_NOT_FOUND:
		default:
			dirent_set_errno(ENOENT);
		}
		return NULL;
	}

	static WIN32_FIND_DATAW* dirent_next(_WDIR* dirp)
	{
		if (dirp->cached) {
			dirp->cached = 0;
			return &dirp->data;
		}

		if (dirp->handle == INVALID_HANDLE_VALUE)
			return NULL;

		if (FindNextFileW(dirp->handle, &dirp->data) == FALSE)
			goto exit_close;

		return &dirp->data;

	exit_close:
		FindClose(dirp->handle);
		dirp->handle = INVALID_HANDLE_VALUE;
		return NULL;
	}

	static DIR* opendir(const char* dirname)
	{
		if (dirname == NULL || dirname[0] == '\0') {
			dirent_set_errno(ENOENT);
			return NULL;
		}

		struct DIR* dirp = (DIR*)malloc(sizeof(struct DIR));
		if (!dirp)
			return NULL;

		wchar_t wname[PATH_MAX + 1];
		size_t n;
		int error = mbstowcs_s(&n, wname, PATH_MAX + 1, dirname, PATH_MAX + 1);
		if (error)
			goto exit_failure;

		dirp->wdirp = _wopendir(wname);
		if (!dirp->wdirp)
			goto exit_failure;

		return dirp;

	exit_failure:
		free(dirp);
		return NULL;
	}

	static struct dirent* readdir(DIR* dirp)
	{
		struct dirent* entry;
		(void)readdir_r(dirp, &dirp->ent, &entry);

		return entry;
	}

	static int readdir_r(
		DIR* dirp, struct dirent* entry, struct dirent** result)
	{
		WIN32_FIND_DATAW* datap = dirent_next(dirp->wdirp);
		if (!datap) {
			*result = NULL;
			return 0;
		}

		size_t n;
		int error = wcstombs_s(
			&n, entry->d_name, PATH_MAX + 1,
			datap->cFileName, PATH_MAX + 1);

		if (error && datap->cAlternateFileName[0] != '\0') {
			error = wcstombs_s(
				&n, entry->d_name, PATH_MAX + 1,
				datap->cAlternateFileName, PATH_MAX + 1);
		}

		if (!error) {
			entry->d_namlen = n - 1;

			DWORD attr = datap->dwFileAttributes;
			if ((attr & FILE_ATTRIBUTE_DEVICE) != 0)
				entry->d_type = DT_CHR;
			else if ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0)
				entry->d_type = DT_DIR;
			else
				entry->d_type = DT_REG;

			entry->d_ino = 0;
			entry->d_off = 0;
			entry->d_reclen = sizeof(struct dirent);
		}
		else {
			entry->d_name[0] = '?';
			entry->d_name[1] = '\0';
			entry->d_namlen = 1;
			entry->d_type = DT_UNKNOWN;
			entry->d_ino = 0;
			entry->d_off = -1;
			entry->d_reclen = 0;
		}

		*result = entry;
		return 0;
	}

	static int closedir(DIR* dirp)
	{
		int ok;

		if (!dirp)
			goto exit_failure;

		ok = _wclosedir(dirp->wdirp);
		dirp->wdirp = NULL;

		free(dirp);
		return ok;

	exit_failure:
		dirent_set_errno(EBADF);
		return -1;
	}

	static void rewinddir(DIR* dirp)
	{
		if (!dirp)
			return;

		_wrewinddir(dirp->wdirp);
	}

	static int scandir(
		const char* dirname, struct dirent*** namelist,
		int (*filter)(const struct dirent*),
		int (*compare)(const struct dirent**, const struct dirent**))
	{
		int result;

		DIR* dir = opendir(dirname);
		if (!dir) {
			return  -1;
		}

		struct dirent* tmp = NULL;
		struct dirent** files = NULL;
		size_t size = 0;
		size_t allocated = 0;
		while (1) {
			if (!tmp) {
				tmp = (struct dirent*)malloc(sizeof(struct dirent));
				if (!tmp)
					goto exit_failure;
			}

			struct dirent* entry;
			if (readdir_r(dir, tmp, &entry) != 0)
				goto exit_failure;

			if (entry == NULL)
				goto exit_success;

			if (filter && !filter(tmp))
				continue;

			if (size >= allocated) {
				size_t num_entries = size * 2 + 16;

				void* p = realloc(files, sizeof(void*) * num_entries);
				if (!p)
					goto exit_failure;

				files = (dirent**)p;
				allocated = num_entries;
			}

			files[size++] = tmp;
			tmp = NULL;
		}

	exit_failure:
		for (size_t i = 0; i < size; i++) {
			free(files[i]);
		}

		free(files);
		files = NULL;

		result =  -1;
		goto exit_status;

	exit_success:
		qsort(files, size, sizeof(void*),
			(int (*) (const void*, const void*)) compare);

		if (namelist)
			*namelist = files;

		result = (int)size;

	exit_status:
		free(tmp);

		closedir(dir);
		return result;
	}

	static int alphasort(const struct dirent** a, const struct dirent** b)
	{
		return strcoll((*a)->d_name, (*b)->d_name);
	}

	static int versionsort(const struct dirent** a, const struct dirent** b)
	{
		return strverscmp((*a)->d_name, (*b)->d_name);
	}

	static int strverscmp(const char* a, const char* b)
	{
		size_t i = 0;
		size_t j;

		while (a[i] == b[i]) {
			if (a[i] == '\0') {
				return 0;
			}
			++i;
		}

		j = i;
		while (j > 0 && isdigit(a[j - 1])) {
			--j;
		}

		if (a[j] == '0' || b[j] == '0') {
			while (a[j] == '0' && a[j] == b[j]) {
				j++;
			}

			if (isdigit(a[j])) {
				if (!isdigit(b[j])) {
					return -1;
				}
			}
			else if (isdigit(b[j])) {
				return 1;
			}
		}
		else if (isdigit(a[j]) && isdigit(b[j])) {
			size_t k1 = j;
			size_t k2 = j;

			while (isdigit(a[k1])) {
				k1++;
			}
			while (isdigit(b[k2])) {
				k2++;
			}

			if (k1 < k2)
				return -1;
			else if (k1 > k2)
				return 1;
		}

		return (int)((unsigned char)a[i]) - ((unsigned char)b[i]);
	}

#if !defined(_MSC_VER) || _MSC_VER < 1400
	static int dirent_mbstowcs_s(
		size_t* pReturnValue, wchar_t* wcstr,
		size_t sizeInWords, const char* mbstr, size_t count)
	{
		size_t n = mbstowcs(wcstr, mbstr, sizeInWords);
		if (wcstr && n >= count)
			return  1;

		if (wcstr && sizeInWords) {
			if (n >= sizeInWords)
				n = sizeInWords - 1;
			wcstr[n] = 0;
		}

		if (pReturnValue) {
			*pReturnValue = n + 1;
		}

		return 0;
	}
#endif

#if !defined(_MSC_VER) || _MSC_VER < 1400
	static int dirent_wcstombs_s(
		size_t* pReturnValue, char* mbstr,
		size_t sizeInBytes, const wchar_t* wcstr, size_t count)
	{
		size_t n = wcstombs(mbstr, wcstr, sizeInBytes);
		if (mbstr && n >= count)
			return 1;

		if (mbstr && sizeInBytes) {
			if (n >= sizeInBytes) {
				n = sizeInBytes - 1;
			}
			mbstr[n] = '\0';
		}

		if (pReturnValue) {
			*pReturnValue = n + 1;
		}

		return 0;
	}
#endif

#if !defined(_MSC_VER) || _MSC_VER < 1400
	static void dirent_set_errno(int error)
	{
		errno = error;
	}
#endif

#ifdef __cplusplus
}
#endif
#endif 
