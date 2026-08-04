const { execSync } = require("child_process");
const path = require("path");
const fs = require("fs");

const CANDIDATE_BUILD_DIRS = [
    "../../build",
    "../../cmake-build-debug",
    "../../cmake-build-release",
    "../../cmake-build",
];

function findCmakeOutputDir() {
    for (const dir of CANDIDATE_BUILD_DIRS) {
        const resolvedPath = path.resolve(path.join(__dirname, dir));
        console.log(`Looking for ${resolvedPath} folder...`);
        if (fs.existsSync(resolvedPath)) {
            return resolvedPath;
        }
    }
    return null;
}

function getTargetExecutable(cmakeOutputDir) {
    const exeName = process.platform === "win32" ? "OpenGitClient.exe" : "OpenGitClient";
    return path.join(cmakeOutputDir, "bin", exeName);
}

function main() {
    const cmakeOutputDir = findCmakeOutputDir();
    if (!cmakeOutputDir) {
        console.log(
            "Failed to find CMake output directory, please have the project in one of the directories listed above"
        );
        process.exit(1);
    }
    console.log(`Using ${cmakeOutputDir} folder`);

    // Run the standard build of the project
    try {
        execSync(`npm run build -- --outDir ${path.join(cmakeOutputDir, "bin", "web")} --emptyOutDir`, {
            stdio: "inherit",
        });
    } catch (err) {
        console.log(`Build failed with exit code ${err.status ?? "unknown"}`);
        process.exit(err.status ?? 1);
    }

    // Check that the project has been built before attempting to run it
    const targetFile = getTargetExecutable(cmakeOutputDir);
    const targetFileLocation = path.resolve(__dirname, targetFile);
    if (!fs.existsSync(targetFileLocation)) {
        console.log(`${targetFile} does not exist, please build the project.`);
        process.exit(1);
    }

    console.log(
        "WARNING: This script does not recompile OpenGitClient. If you have made any changes to " +
        "the C++/C# backend code, they will not have taken effect - please recompile if needed."
    );
    console.log("Running OpenGitClient...");

    try {
        execSync(targetFileLocation, { stdio: "inherit" });
    } catch (err) {
        console.log(`OpenGitClient exited with code ${err.status ?? "unknown"}`);
        process.exit(err.status ?? 1);
    }
}

main();
