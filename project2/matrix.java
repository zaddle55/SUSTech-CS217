// Purpose: Test the speed of matrix multiplication.

class Matrix {
    private float[][] matrix;
    private int rows;
    private int cols;

    public Matrix(int rows, int cols) {
        this.rows = rows;
        this.cols = cols;
        this.matrix = new float[rows][cols];
    }

    public Matrix(float[][] matrix) {
        this.rows = matrix.length;
        this.cols = matrix[0].length;
        this.matrix = matrix;
    }

    public Matrix randomize() {
        for (int i = 0; i < this.rows; i++) {
            for (int j = 0; j < this.cols; j++) {
                this.matrix[i][j] = (float) Math.random() * 2 - 1;
            }
        }
        return this;
    }

    public Matrix multiply(Matrix matrix) {
        if (this.cols != matrix.rows) {
            System.out.println("Columns of A must match rows of B.");
            return null;
        }

        Matrix result = new Matrix(this.rows, matrix.cols);

        for (int i = 0; i < result.rows; i++) {
            for (int j = 0; j < result.cols; j++) {
                float sum = 0;
                for (int k = 0; k < this.cols; k++) {
                    sum += this.matrix[i][k] * matrix.matrix[k][j];
                }
                result.matrix[i][j] = sum;
            }
        }

        return result;
    }
}

class Test {
    public static void test_multiply_speed1() {
        Matrix a = new Matrix(1000, 1000).randomize();
        Matrix b = new Matrix(1000, 1000).randomize();

        long startTime = System.currentTimeMillis();
        Matrix c = a.multiply(b);
        long endTime = System.currentTimeMillis();

        System.out.println("Time taken: " + (endTime - startTime) + "ms");
    }

    public static void test_multiply_speed2() {
        double time_sum = 0;
        int test_n = 100;

        for (int i = 0; i < test_n; i++) {
            Matrix a = new Matrix(100, 100).randomize();
            Matrix b = new Matrix(100, 100).randomize();

            long startTime = System.currentTimeMillis();
            Matrix c = a.multiply(b);
            long endTime = System.currentTimeMillis();

            time_sum += (endTime - startTime);
        }

        System.out.println("Average time taken: " + time_sum / test_n + "ms");
    }
}

public class Main {
    public static void main(String[] args) {
        Test.test_multiply_speed1();
        Test.test_multiply_speed2();
    }
}