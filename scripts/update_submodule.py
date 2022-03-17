import os
import repository_directories

if __name__ == "__main__":
    # working directory where we start our script
    content_directory = repository_directories.get_content_directory()        

    # add new submodules here that need to be updated when we run this script
    # current submodules updated
    #       * rex-assets
    submodules = [content_directory]

    for submodule in submodules:
        # print the submodule that needs to be updated
        print("\t" + submodule)
        # update given submodule
        os.system("git submodule update --remote " + submodule)