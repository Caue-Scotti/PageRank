import java.util.Random;
import java.io.BufferedWriter;
import java.io.FileWriter;
import java.io.IOException;

/*
    USAGE: 
        java DataGen <matrix_size> <file_name>
*/


public class DataGen {
    public static void main(String[] args) {
        
        int size = Integer.parseInt(args[0]);
        String file_name = args[1];
        Random random = new Random();
        try (BufferedWriter writer = new BufferedWriter(new FileWriter(String.format("test_data/%s.txt", file_name), true))){
            writer.write(String.valueOf(size));
            writer.newLine();
            for (int i = 0; i < size; i++){
                for (int j = 0; j < size; j++){
                    writer.write(String.valueOf(random.nextInt(2)) + " ");
                }
                writer.newLine();
            }
        } 
        catch (IOException e) {
            e.printStackTrace();
        }
    }
}