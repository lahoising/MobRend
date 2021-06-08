import sys
import subprocess
import os

def rreplace(string, oldSubstring, newSubstring, occurence):
    li = string.rsplit(oldSubstring)
    return newSubstring.join(li)

def main():
    
    resources_dir = sys.argv[1]
    glslangValidator = sys.argv[2]

    supported_shaders = ( '.vert', '.tesc', '.tese', '.geom', '.frag', '.comp' )
    shaders = [ f.path for f in os.scandir(resources_dir) 
                if f.is_file() and f.path.endswith(supported_shaders) ]

    for source in shaders:
        out_spirv_file = source
        
        for file_extension in supported_shaders:
            if out_spirv_file.endswith(file_extension):
                out_spirv_file = rreplace(out_spirv_file, file_extension, '.spv', 1)
                break

        completed = subprocess.run(
            [glslangValidator, source, '-o', out_spirv_file],
            stdout=subprocess.PIPE,)

        print(completed.stdout.decode('utf-8'))

if __name__ == "__main__":
    main()