import os

# File extensions to delete (lowercase)
EXTENSIONS_TO_DELETE = {".p3d", ".paa", ".bin", ".ogg"}

# Set to False to actually delete files
DRY_RUN = False

def delete_files_recursively(root_dir):
    for dirpath, dirnames, filenames in os.walk(root_dir):
        for filename in filenames:
            file_ext = os.path.splitext(filename)[1].lower()
            if file_ext in EXTENSIONS_TO_DELETE:
                full_path = os.path.join(dirpath, filename)
                if DRY_RUN:
                    print(f"[DRY RUN] Would delete: {full_path}")
                else:
                    try:
                        os.remove(full_path)
                        print(f"Deleted: {full_path}")
                    except Exception as e:
                        print(f"Failed to delete {full_path}: {e}")

if __name__ == "__main__":
    start_directory = os.getcwd()
    print(f"Starting in: {start_directory}")
    delete_files_recursively(start_directory)

    if DRY_RUN:
        print("\nDry run complete. No files were deleted.")
        print("Set DRY_RUN = False to actually delete files.")
