import sys
import subprocess
import os

def main():
    
    # resources_dir = sys.argv[1]
    glslangValidator = sys.argv[1]

    resource_dirs = []
    for i in range(2, len(sys.argv)):
        resource_dirs.append(sys.argv[i])

    supported_shaders = ( '.vert', '.tesc', '.tese', '.geom', '.frag', '.comp' )
    shaders = []
    for res_dir in resource_dirs:
        shaders = shaders + [ f.path for f in os.scandir(res_dir) 
                    if f.is_file() and f.path.endswith(supported_shaders) ]

    for source in shaders:
        out_spirv_file = source + '.spv'
        if os.path.exists(out_spirv_file) and os.stat(out_spirv_file).st_mtime > os.stat(source).st_mtime:
            continue

        print(out_spirv_file)
        completed = subprocess.run(
            [glslangValidator, '-V', source, '-o', out_spirv_file],
            stdout=subprocess.PIPE,)

        print(completed.stdout.decode('utf-8'), end='')

if __name__ == "__main__":
    main()
