#pragma once

#include "radiant/vfs/Doom3FileSystem.h"

// Fixture for tests that make use of the VFS
struct VFSFixture
{
    // The Doom3FileSystem under test
    vfs::Doom3FileSystem fs;

    // Initialisation parameters for the Doom3FileSystem
    vfs::VirtualFileSystem::ExtensionSet pakExtensions;
    vfs::SearchPaths searchPaths;

    // Get the srcdir environment variable (set by Automake)
    std::string srcdir() const
    {
        const char* envVal = getenv("srcdir");
        if (envVal)
            return std::string(envVal);
        else
            throw std::runtime_error("srcdir not set");
    }

    VFSFixture()
    {
        // Setup the output stream
        GlobalOutputStream().setStream(std::cout);

        // Configure search paths and extensions
        pakExtensions.insert("pk4");
        searchPaths.insertIfNotExists(srcdir() + "/test/data/vfs_root");

        // Initialise the VFS
        fs.initialise(searchPaths, pakExtensions);
    }
};
