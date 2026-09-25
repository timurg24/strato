#include "Wrangler/Filesystem/Filesystem.hpp"

// error
#include <tul/ErrorOps.hpp>

// tul
#include <tul/FileOps.hpp>
#include <tul/StringOps.hpp>


// 3rd party
#include <physfs.h>

Wrangler::Filesystem::Filesystem(const char *argv0, const std::filesystem::path& contentPath)
{
    init(argv0, contentPath);
}

/// @brief Initializes the Filesystem
/// @param argv argv[0] from the main function
/// @param contentPath Path to your game file (eg gamefiles/game.pak)
/// @note Crashes on failure and displays an error message+
void Wrangler::Filesystem::init(const char *argv0, const std::filesystem::path& contentPath)
{
    if(!std::filesystem::exists(contentPath)) tul::FatalError({"Failed to locate game content: ", contentPath.string().c_str()});

    std::filesystem::path contentDir = contentPath.parent_path();

    if(!PHYSFS_init(argv0))
        tul::FatalError({
            "Failed to initialize PhysFS: ",
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())
        });
    tul::Print({"[Filesystem] PhyFS initialized\n"});

    std::filesystem::path engineContent =
        contentDir / "Wrangler - Assets.pak";
    if (!PHYSFS_mount(engineContent.string().c_str(), nullptr, 1))
    {
        tul::FatalError({
            "Failed to mount engine content: ",
            engineContent.string().c_str(),
            "\nPhysFS Error: ",
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())
        });
    } else tul::Print({"[Filesystem] Mounted engine content\n"});


    if (!PHYSFS_mount(contentPath.string().c_str(), nullptr, 1))
    {
        tul::FatalError({
            "Failed to mount game content: ",
            contentPath.string().c_str(),
            "\nPhysFS Error: ",
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())
        });
    }
    else
    {
        tul::Print({"[Filesystem] Mounted game content\n"});
    }

    // mods
    if(std::filesystem::exists(contentDir / "MODS.txt")) {
         std::vector<std::string> lines = tul::SplitString(tul::ReadFile(contentDir / "MODS.txt"), '\n');
        if(!lines[0].empty()) tul::Print({"[Assets] Mounted mods:\n"});

        for(const auto& line : lines) {
            if(line.empty()) continue;
            if(!PHYSFS_mount((contentDir / line).string().c_str(), "/", 0)) tul::FatalError({"Couldn't mount mod package: ", line.c_str(), "\nPhysFS Error:", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())});
            tul::Print({"\t", line.c_str()});
        }
        tul::Print({"\n"});
    }
}

/// @brief Reads a file
/// @param path Path
/// @return File object
const Wrangler::File Wrangler::Filesystem::readFile(const std::string &path) const
{
    PHYSFS_File* handle = PHYSFS_openRead(path.c_str());

    if (!handle)
        tul::Alert({"Couldn't open file in archive: ", path});

    const PHYSFS_sint64 size = PHYSFS_fileLength(handle);

    if (size < 0)
    {
        PHYSFS_close(handle);

        tul::Alert({"Couldn't read size of file in archive: ", path});
    }

    std::vector<byte> data(
        static_cast<size_t>(size)
    );

    const PHYSFS_sint64 bytesRead =
        PHYSFS_readBytes(
            handle,
            data.data(),
            static_cast<PHYSFS_uint64>(data.size())
        );

    PHYSFS_close(handle);

    if (bytesRead != size)
    {
        tul::Alert({"Couldn't read full file in archive: ", path});
    }

    return File(std::move(data));
}

Wrangler::Filesystem::~Filesystem()
{
    PHYSFS_deinit();
}
