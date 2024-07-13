#include "zz/palette.h"

#include "zz/log.h"
#include "zz/memory.h"
#include "zz/filesystem.h"

b8 zz_palette_file_create(struct zz_palette_file* file, struct zz_palette_file_config* config)
{
    file->size = zz_filesystem_get_size(config->path);
    if (file->size < sizeof(struct zz_palette_file_header))
    {
        ZZ_LOG_ERROR("Palette file does not have a complete header.");
        return ZZ_FALSE;
    }

    file->data = zz_memory_allocate(file->size, ZZ_MEMORY_TAG_ASSET);
    zz_filesystem_load(config->path, file->data, file->size);

    file->header = (struct zz_palette_file_header*)file->data;
    file->body = (struct zz_palette_file_body_element*)(((struct zz_palette_file_header*)file->data) + 1);

#ifdef ZZ_LITTLE_ENDIAN
    file->header->width = ZZ_FROM_BIG_ENDIAN_U16(file->header->width);
    file->header->height = ZZ_FROM_BIG_ENDIAN_U16(file->header->height);
#endif

    return ZZ_TRUE;
}

void zz_palette_file_destroy(struct zz_palette_file* file)
{
    zz_memory_deallocate(file->data, file->size, ZZ_MEMORY_TAG_ASSET);
    file->size = 0;
    file->data = ZZ_NULL;
    file->header = ZZ_NULL;
    file->body = ZZ_NULL;
}

b8 zz_palette_create(struct zz_palette* palette, struct zz_palette_config* config)
{
    struct zz_palette_file_config file_config;
    file_config.path = config->path;
    if (!zz_palette_file_create(&palette->file, &file_config))
    {
        return ZZ_FALSE;
    }

    return ZZ_TRUE;
}

void zz_palette_destroy(struct zz_palette* palette)
{
    zz_palette_file_destroy(&palette->file);
}