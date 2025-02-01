import os
import shutil
import sys

def preprocess_glsl_files(shaders_folder):
    # Set up directories
    includes_folder = os.path.join(shaders_folder, 'includes')
    temp_folder = os.path.join(shaders_folder, 'temp')
    os.makedirs(temp_folder, exist_ok=True)

    # Loop through all GLSL files in shaders folder
    for root, dirs, files in os.walk(shaders_folder):
        for file in files:
            if file.endswith('.glsl'):
                shader_path = os.path.join(root, file)
                temp_shader_path = os.path.join(temp_folder, os.path.relpath(shader_path, shaders_folder))

                # Create the directory structure in temp folder
                os.makedirs(os.path.dirname(temp_shader_path), exist_ok=True)

                with open(shader_path, 'r') as shader_file:
                    content = shader_file.read()

                # Replace #include statements with file contents and banners
                for include_statement in content.splitlines():
                    if include_statement.startswith('#include'):
                        # Extract the file name from the include statement
                        include_file = include_statement.split('"')[1]
                        include_path = os.path.join(includes_folder, include_file)

                        if os.path.exists(include_path):
                            with open(include_path, 'r') as include_file_content:
                                include_content = include_file_content.read()

                            # Add banner comments with the included file name
                            banner_comment = f"\n// ---- Start of {include_file} ----\n"
                            banner_comment += include_content
                            banner_comment += f"\n// ---- End of {include_file} ----\n"

                            # Replace the #include statement with the bannered content
                            content = content.replace(include_statement, banner_comment)

                # Write the processed content to the temp folder
                with open(temp_shader_path, 'w') as temp_shader_file:
                    temp_shader_file.write(content)

    print(f"Shaders have been preprocessed and saved in {temp_folder}")

# Get the shaders folder path from the command line argument
if len(sys.argv) < 2:
    print("Usage: python preprocess_glsl.py <shaders_folder>")
else:
    shaders_folder = sys.argv[1]
    preprocess_glsl_files(shaders_folder)
