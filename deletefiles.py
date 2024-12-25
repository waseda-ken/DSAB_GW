import os

def main():
    # List of files to delete
    files_to_delete = [
        "orgdata", "dec_13", "decdata", "enc_13", "encdata", 
        "eval", "gen", "seq", "seqdata", "syn", "syndna"
    ]
    
    for file in files_to_delete:
        try:
            os.remove(file)
            print(f"Deleted {file}")
        except FileNotFoundError:
            print(f"{file} not found")
        except Exception as e:
            print(f"Error deleting {file}: {e}")

if __name__ == "__main__":
    main()