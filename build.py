import subprocess as sp
import os
import platform
import argparse
import shutil
import zipfile

env = os.environ.copy()

def _prep_binary_deps(target_arch):
    platform = detect_platform()
    if target_arch == "arm64" and platform == "Windows":
        print("v8 prebuilt arm64 binaries are not available for windows")
        exit(1)

    # TODO: Update the URLs to nexus.vicionline.com once the DNS records are established
    # In the meantime, build while connected to the VPN
    win_urls = {
        "libcxx": f"http://192.168.8.69:8081/repository/vici-dep-binaries/libcxx/Windows/{target_arch}/19.0.0/libcxx.zip",
        "v8": f"http://192.168.8.69:8081/repository/vici-dep-binaries/v8/Windows/{target_arch}/12.7.224.18/v8.zip",
        "clang": f"http://192.168.8.69:8081/repository/vici-dep-binaries/clang/Windows/{target_arch}/19.0.0/clang.zip"
    }
    darwin_urls = {
        "v8": f"http://192.168.8.69:8081/repository/vici-dep-binaries/v8/Darwin/Sonoma/{target_arch}/12.7.224.16/v8.zip"
    }

    if (platform == "Windows"):
        urls = win_urls
    elif (platform == "Darwin"):
        urls = darwin_urls

    
    script_location = get_script_location()

    # make third_party directory if it does not exist
    if not os.path.exists(f"{script_location}/third_party"):
        os.makedirs(f"{script_location}/third_party")

    def rm_old(dep, target_arch):
        file_name = f"{script_location}/third_party/{dep}_{target_arch}.zip"
        if os.path.exists(file_name):
            os.remove(f"{script_location}/third_party/{dep}_{target_arch}.zip")
        folder_name = f"{script_location}/third_party/{dep}"
        if os.path.exists(folder_name):
            shutil.rmtree(folder_name)

    # remove old files if the url has changed
    for dep, url in urls.items():
        if os.path.exists(f"{script_location}/third_party/{dep}/url.txt"):
            url_file_contents = ""
            with open(f"{script_location}/third_party/{dep}/url.txt", "r") as f:
                url_file_contents = f.read().strip()
            if url_file_contents != url:
                rm_old(dep, target_arch)
        else:
            rm_old(dep, target_arch)

    # download the files if they do not exist
    for dep, url in urls.items():
        if not os.path.exists(f"{script_location}/third_party/{dep}_{target_arch}.zip"):
            os.system(f"curl -L {url} -o \"{script_location}/third_party/{dep}_{target_arch}.zip\"")
        if not os.path.exists(f"{script_location}/third_party/{dep}"):
            with zipfile.ZipFile(f"{script_location}/third_party/{dep}_{target_arch}.zip", 'r') as zip_ref:
                zip_ref.extractall(f"{script_location}/third_party/{dep}") 
            # add a file containing the url so we can check if the file is up to date  
            with open(f"{script_location}/third_party/{dep}/url.txt", "w") as f:
                f.write(url)

def _prep_playfab():
    def _prep_playfab_windows():
        os.system("set-gitmodules.bat")
    
    def _prep_playfab_unix():
        os.system(f"chmod +x {get_script_location()}/third_party/XPlatCppSdk/set-gitmodules.sh")
        os.system("./set-gitmodules.sh")

    os.chdir(f"{get_script_location()}/third_party/XPlatCppSdk")
    if detect_platform() == "Windows":
        _prep_playfab_windows()
    else:
        _prep_playfab_unix()
    os.chdir(get_script_location())

def _generate_vicigen_headers(platform):
    """
    Generate headers from all the files inside the ViciClient/files dir with xxd
    """
    def xxd_windows(script_loc, file):
        sp.run(f"\"C:\\Program Files\\Git\\git-bash.exe\" -c \"xxd -i {file} > {script_loc}/ViciClient/vicigen/{file}.h\"", shell=True)
    
    def xxd_unix(script_loc, file):
        os.system(f"xxd -i {file} > {script_loc}/ViciClient/vicigen/{file}.h")


    script_loc = get_script_location()
    if not os.path.exists(f"{script_loc}/ViciClient/vicigen"):
        os.makedirs(f"{script_loc}/ViciClient/vicigen")
    os.chdir(f"{script_loc}/ViciClient/files")
    for file in os.listdir():
        if (platform == "Windows"):
            xxd_windows(script_loc, file)
        else:
            xxd_unix(script_loc, file)
    os.chdir(script_loc)

def _setup_env_windows():
    if env.get("VSINSTALLDIR") is None:
        raise ValueError("\"VSINSTALLDIR\" environment variable is not set. Ensure you have Visual Studio installed and set the env variable.")
    if not str(env["VSINSTALLDIR"]).endswith("\\"):
        env["VSINSTALLDIR"] += "\\"
    print("Current VS Installation Directory:", env["VSINSTALLDIR"])

def _append_cmake_args_windows(cmd, args):
    vcpkg_target_triplet = "x64-windows-clang" if get_target_arch(args.arch) == "x64" else None
    if vcpkg_target_triplet is not None:
        cmd += f' -DVCPKG_TARGET_TRIPLET={vcpkg_target_triplet}'
    cmd += f' -DVCPKG_OVERLAY_TRIPLETS="{get_script_location()}/cmake/triplets"'
    return cmd

def _copy_custom_cmake_files_windows():
    shutil.copy(f"{get_script_location()}/cmake/custom_cmake_files/XPlatCppSdkWindows_CMakeLists.txt", f"{get_script_location()}/third_party/XPlatCppSdk/build/Windows/CMakeLists.txt")
    shutil.copy(f"{get_script_location()}/cmake/custom_cmake_files/LibJsonWindows_CMakeLists.txt", f"{get_script_location()}/third_party/XPlatCppSdk/external/jsoncpp/CMakeLists.txt")

def _prep_final_cmake_cmd_windows(cmd):
    # replace all "\"" with "^\""
    cmd = cmd.replace("\"", "^\"")
    return cmd

def _run_cmake_generation(args):
    platform = detect_platform()
    build_cmd = f'cmake --build "{generate_build_dir(args)}" --config {"Release" if args.release else "Debug"}'

    _generate_vicigen_headers(platform)

    cmd = f'cmake -S "{get_script_location()}" -B "{generate_build_dir(args)}" -G{args.generator}'
    cmd += f' -DCMAKE_BUILD_TYPE={"Release" if args.release else "Debug"}'
    
    if platform == "Windows":
        _setup_env_windows()
        _copy_custom_cmake_files_windows()
        cmd = _append_cmake_args_windows(cmd, args)
    elif platform == "Darwin":
        cmd += f' -DCMAKE_OSX_ARCHITECTURES={get_cmake_target_arch(args.arch)}'
        cmd += ' -DCMAKE_OSX_DEPLOYMENT_TARGET=14.1'

    if platform == "Windows":
        cmd = _prep_final_cmake_cmd_windows(cmd)
        sp_args = [f'"{get_script_location()}/_build.bat"', f'"{get_script_location()}"', f'"{cmd}"']
        if args.build:
            sp_args.append(f'"{build_cmd}"')
        sp.run(" ".join(sp_args), shell=True, env=env)
        return
    elif platform == "Darwin":
        print(cmd)
        sp.run(cmd, shell=True)

    if args.build:
        print(build_cmd)
        sp.run(build_cmd, shell=True, env=env)

def generate_build_dir(args):
    build_dir = f"{get_script_location()}/out/{detect_platform()}/{get_target_arch(args.arch)}/{'Release' if args.release else 'Debug'}"
    if not os.path.exists(build_dir):
        os.makedirs(build_dir)
    return build_dir

def get_script_location():
    """
    Get the location of the script
    """
    path = os.path.dirname(os.path.realpath(__file__))
    # return with all backslashes replaced with forward slashes
    return path.replace("\\", "/")

def detect_platform():
    """
    Return the platform name
    """
    return platform.system()

def get_current_arch():
    """
    Get the current architecture
    e.g. x64, arm64
    """
    arch = platform.machine()
    return get_target_arch(arch) # same translation

def get_target_arch(arch_in):
    """
    Get the target architecture
    """
    if arch_in.lower() in ["x64", "amd64"]:
        return "x64"
    elif arch_in.lower() in ["arm64"]:
        return "arm64"

def get_cmake_target_arch(arch_in):
    """
    Get the target architecture for CMake
    """
    if arch_in.lower() in ["x64", "amd64"]:
        return "x86_64"
    elif arch_in.lower() in ["arm64"]:
        return "arm64"

def _clean():
    if os.path.exists(f"{get_script_location()}/out"):
        shutil.rmtree(f"{get_script_location()}/out")
        print("Cleaned build directory.")
    else:
        print("Build directory does not exist. Already clean.")
    exit(0)

def _build(args):
    if args.clean:
        _clean()
    os.system("git submodule update --init --recursive")
    _prep_binary_deps(get_target_arch(args.arch))
    _prep_playfab()
    _run_cmake_generation(args)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Build script for the project.")

    if detect_platform() == "Windows":
        default_generator = "Ninja"
    elif detect_platform() == "Darwin":
        default_generator = "Xcode"
    else:
        default_generator = "Ninja"

    parser.add_argument(
        "-g",
        "--generator",
        dest="generator",
        type=str,
        default=default_generator,
        help="Specify the generator for CMake (default ninja)."
    )
    parser.add_argument(
        "-a",
        "--arch",
        dest="arch",
        type=str,
        default=f"{get_current_arch()}",
        choices=["x64", "arm64"],
        help="defaults to current architecture"
    )
    parser.add_argument(
        "-b",
        "--build",
        dest="build",
        action="store_true",
        default=True,
        help="Build the project (default true)."
    )

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "-r",
        "--release",
        dest="release",
        action="store_true",
        default=False,
    )
    group.add_argument(
        "-d",
        "--debug",
        dest="debug",
        action="store_true",
        default=False,
    )
    group.add_argument(
        "-c",
        "--clean",
        dest="clean",
        action="store_true",
        help="Clean the build directory (default false)."
    )
    parser.add_argument_group
    args = parser.parse_args()
    _build(args)

    