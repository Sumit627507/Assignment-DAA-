
import java.io.*;
import java.util.*;

public class QuickSort {

    static final int MAX_MOVIES = 100000;
    static final int NAME_LEN = 100;

    static class Movie {
        int id;
        String title;
        int releaseYear;
        float imdbRating;
        long watchTimeMinutes;

        Movie(int id, String title, int releaseYear,
              float imdbRating, long watchTimeMinutes) {
            this.id = id;
            this.title = title;
            this.releaseYear = releaseYear;
            this.imdbRating = imdbRating;
            this.watchTimeMinutes = watchTimeMinutes;
        }
    }

    static Movie[] movies = new Movie[MAX_MOVIES];
    static int movieCount = 0;

    // Compare two movies
    static int compare(Movie a, Movie b, int key) {

        if (key == 1) {

            if (a.imdbRating > b.imdbRating)
                return 1;

            if (a.imdbRating < b.imdbRating)
                return -1;

            return 0;

        } else if (key == 2) {

            if (a.releaseYear > b.releaseYear)
                return 1;

            if (a.releaseYear < b.releaseYear)
                return -1;

            return 0;

        } else if (key == 3) {

            if (a.watchTimeMinutes > b.watchTimeMinutes)
                return 1;

            if (a.watchTimeMinutes < b.watchTimeMinutes)
                return -1;

            return 0;
        }

        return 0;
    }

    // Swap two movies
    static void swapMovies(Movie a, Movie b) {

        // Not used because Java references are passed by value.
        // We swap array elements directly in quickSort.
    }

    // Partition
    static int partition(Movie[] arr, int low, int high, int key) {

        Movie pivot = arr[high];

        int i = low - 1;

        for (int j = low; j < high; j++) {

            if (compare(arr[j], pivot, key) >= 0) {

                i++;

                Movie temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }

        Movie temp = arr[i + 1];
        arr[i + 1] = arr[high];
        arr[high] = temp;

        return i + 1;
    }

    // Quick Sort
    static void quickSort(Movie[] arr, int low, int high, int key) {

        if (low < high) {

            int pi = partition(arr, low, high, key);

            quickSort(arr, low, pi - 1, key);

            quickSort(arr, pi + 1, high, key);
        }
    }

    // Load dataset from CSV file
    static int loadDatasetFromFile(String filename) {

        int idx = 0;

        try {

            BufferedReader br = new BufferedReader(
                    new FileReader(filename)
            );

            // Skip header
            br.readLine();

            String line;

            while ((line = br.readLine()) != null
                    && idx < MAX_MOVIES) {

                String[] data = line.split(",");

                if (data.length < 4)
                    continue;

                String title = data[0];

                int releaseYear =
                        Integer.parseInt(data[1]);

                float imdbRating =
                        Float.parseFloat(data[2]);

                long watchTimeMinutes =
                        Long.parseLong(data[3]);

                movies[idx] = new Movie(
                        idx + 1,
                        title,
                        releaseYear,
                        imdbRating,
                        watchTimeMinutes
                );

                idx++;
            }

            br.close();

        } catch (FileNotFoundException e) {

            return 0;

        } catch (IOException e) {

            System.out.println("Error reading file.");

            return 0;

        } catch (NumberFormatException e) {

            System.out.println("Invalid data in CSV file.");

            return 0;
        }

        return idx;
    }

    // Print movies
    static void printMovies(int limit) {

        int n = limit;

        if (n > movieCount)
            n = movieCount;

        System.out.printf(
                "%-5s %-30s %-8s %-8s %-12s%n",
                "ID",
                "Title",
                "Year",
                "Rating",
                "WatchTime"
        );

        for (int i = 0; i < n; i++) {

            System.out.printf(
                    "%-5d %-30s %-8d %-8.1f %-12d%n",
                    movies[i].id,
                    movies[i].title,
                    movies[i].releaseYear,
                    movies[i].imdbRating,
                    movies[i].watchTimeMinutes
            );
        }
    }

    public static void main(String[] args) {

        Scanner sc = new Scanner(System.in);

        System.out.println(
                "StreamFlix Movie Recommendation Sorter"
        );

        System.out.print(
                "Enter CSV filename (format: title,year,rating,watchtime): "
        );

        String filename = sc.next();

        movieCount = loadDatasetFromFile(filename);

        if (movieCount == 0) {

            System.out.println(
                    "File not found or empty."
            );

            sc.close();
            return;
        }

        System.out.println(
                "\nLoaded " + movieCount + " movies.\n"
        );

        System.out.println("Sort by:");
        System.out.println("1. IMDB Rating");
        System.out.println("2. Release Year");
        System.out.println("3. Watch Time Popularity");

        System.out.print("Enter choice: ");

        int key = sc.nextInt();

        if (key < 1 || key > 3) {

            System.out.println("Invalid choice.");
            sc.close();
            return;
        }

        // Quick Sort
        quickSort(
                movies,
                0,
                movieCount - 1,
                key
        );

        System.out.print(
                "\nHow many top results to display? "
        );

        int displayCount = sc.nextInt();

        System.out.println(
                "\nSorted Recommendations:\n"
        );

        printMovies(displayCount);

        sc.close();
    }
}
