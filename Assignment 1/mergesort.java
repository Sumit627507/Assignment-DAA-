
import java.io.*;

public class MergeSort {

    static final int MAX_ROWS = 10000;

    // Merge two sorted parts
    static void merge(int[] arr, int[] temp, int left, int mid, int right) {

        int i = left;
        int j = mid + 1;
        int k = left;

        while (i <= mid && j <= right) {

            if (arr[i] < arr[j])
                temp[k++] = arr[i++];
            else
                temp[k++] = arr[j++];
        }

        while (i <= mid)
            temp[k++] = arr[i++];

        while (j <= right)
            temp[k++] = arr[j++];

        // Copy temp back to arr
        for (i = left; i <= right; i++)
            arr[i] = temp[i];
    }

    // Merge Sort
    static void mergeSort(int[] arr, int[] temp, int left, int right) {

        if (left >= right)
            return;

        int mid = (left + right) / 2;

        mergeSort(arr, temp, left, mid);
        mergeSort(arr, temp, mid + 1, right);

        merge(arr, temp, left, mid, right);
    }

    public static void main(String[] args) {

        String filePath = "social_media_usage.csv";

        try {
            BufferedReader br = new BufferedReader(new FileReader(filePath));

            int choice;

            System.out.println("Which column do you want to sort?");
            System.out.println("1. Daily_Minutes_Spent");
            System.out.println("2. Posts_Per_Day");
            System.out.println("3. Likes_Per_Day");
            System.out.println("4. Follows_Per_Day");
            System.out.print("Enter your choice: ");

            java.util.Scanner sc = new java.util.Scanner(System.in);
            choice = sc.nextInt();

            // Skip header
            br.readLine();

            int[] values = new int[MAX_ROWS];
            int[] temp = new int[MAX_ROWS];

            int count = 0;

            String line;

            while ((line = br.readLine()) != null) {

                String[] data = line.split(",");

                if (data.length < 6)
                    continue;

                String user = data[0];
                String app = data[1];

                int dailyMinutes = Integer.parseInt(data[2]);
                int posts = Integer.parseInt(data[3]);
                int likes = Integer.parseInt(data[4]);
                int follows = Integer.parseInt(data[5]);

                switch (choice) {

                    case 1:
                        values[count] = dailyMinutes;
                        break;

                    case 2:
                        values[count] = posts;
                        break;

                    case 3:
                        values[count] = likes;
                        break;

                    case 4:
                        values[count] = follows;
                        break;

                    default:
                        System.out.println("Invalid choice.");
                        br.close();
                        sc.close();
                        return;
                }

                count++;

                if (count >= MAX_ROWS)
                    break;
            }

            br.close();
            sc.close();

            // Apply Merge Sort
            mergeSort(values, temp, 0, count - 1);

            System.out.println("\nSorted Values:");

            for (int i = 0; i < count; i++)
                System.out.print(values[i] + " ");

        } catch (FileNotFoundException e) {

            System.out.println("Cannot open file.");

        } catch (IOException e) {

            System.out.println("Error reading file.");

        } catch (NumberFormatException e) {

            System.out.println("Invalid number in CSV file.");
        }
    }
}
