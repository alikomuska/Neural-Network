import matplotlib.pyplot as plt

# Read and process data from centers3.txt and processed_data.txt
def read_data(file_name):
    data = []
    with open(file_name, 'r') as file:
        for line in file:
            row = line.strip().split()
            if len(row) >= 2:  # Checking for at least two columns of data
                data.append((float(row[0]), float(row[1])))
            else:
                print(f"Issue with data format in {file_name}: {row}")
    return data

# Read data
centers_data = read_data('centers9.txt')
processed_data = read_data('processed_data.txt')

# Extract x, y values for each dataset
centers_x, centers_y = zip(*centers_data)
processed_x, processed_y = zip(*processed_data)

# Plot the data
plt.figure(figsize=(8, 6))

# Plot data from centers3.txt with '*' symbol and larger size without a legend entry
plt.scatter(centers_x, centers_y, marker='*', s=150, label='_nolegend_')

# Plot data from processed_data.txt with '+' symbol and smaller size with legend entry
plt.scatter(processed_x, processed_y, marker='+', s=50, label='Processed Data')

# Customize plot
plt.title('Centers 9')
plt.xlabel('X-axis')
plt.ylabel('Y-axis')
plt.legend()

# Save or show the plot
plt.savefig('Centers9.png')  # Save the plot as an image
plt.show()  # Show the plot
