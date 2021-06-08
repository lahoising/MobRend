import sys
import subprocess
import os

def main():
    
    resources_dir = sys.argv[1]
    glslangValidator = sys.argv[2]

    supported_shaders = ( '.vert', '.tesc', '.tese', '.geom', '.frag', '.comp' )
    shaders = [ f.path for f in os.scandir(resources_dir) 
                if f.is_file() and f.path.endswith(supported_shaders) ]

    for source in shaders:
        out_spirv_file = source
        
        lastPeriod = out_spirv_file.find('.')
        out_spirv_file = out_spirv_file[:lastPeriod] + '_' + out_spirv_file[lastPeriod+1:] + '.spv'

        print(out_spirv_file)
        completed = subprocess.run(
            [glslangValidator, '-V', source, '-o', out_spirv_file],
            stdout=subprocess.PIPE,)

        print(completed.stdout.decode('utf-8'))

if __name__ == "__main__":
    main()