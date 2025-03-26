import java.util.Random;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;

public class Dotproduct {
    private static final Random random = new Random();
    private static int n_threads = 1;

    // Vector creation methods
    public static byte[] createVectorByte(int size) {
        byte[] vec = new byte[size];
        return vec;
    }

    public static short[] createVectorShort(int size) {
        short[] vec = new short[size];
        return vec;
    }

    public static int[] createVectorInt(int size) {
        int[] vec = new int[size];
        return vec;
    }

    public static float[] createVectorFloat(int size) {
        float[] vec = new float[size];
        return vec;
    }

    public static double[] createVectorDouble(int size) {
        double[] vec = new double[size];
        return vec;
    }

    // Random initialization methods
    public static void randomizeByte(byte[] vec, byte start, byte end) {
        for (int i = 0; i < vec.length; i++) {
            vec[i] = (byte) (random.nextInt(end - start + 1) + start);
        }
    }

    public static void randomizeShort(short[] vec, short start, short end) {
        for (int i = 0; i < vec.length; i++) {
            vec[i] = (short) (random.nextInt(end - start + 1) + start);
        }
    }

    public static void randomizeInt(int[] vec, int start, int end) {
        for (int i = 0; i < vec.length; i++) {
            vec[i] = random.nextInt(end - start + 1) + start;
        }
    }

    public static void randomizeFloat(float[] vec, float start, float end) {
        for (int i = 0; i < vec.length; i++) {
            vec[i] = random.nextFloat() * (end - start) + start;
        }
    }

    public static void randomizeDouble(double[] vec, double start, double end) {
        for (int i = 0; i < vec.length; i++) {
            vec[i] = random.nextDouble() * (end - start) + start;
        }
    }

    // Plain dot product methods
    public static long dotProductPlainByte(byte[] a, byte[] b, int size) {
        long result = 0;
        for (int i = 0; i < size; i++) {
            result += a[i] * b[i];
        }
        return result;
    }

    public static long dotProductPlainShort(short[] a, short[] b, int size) {
        long result = 0;
        for (int i = 0; i < size; i++) {
            result += a[i] * b[i];
        }
        return result;
    }

    public static long dotProductPlainInt(int[] a, int[] b, int size) {
        long result = 0;
        for (int i = 0; i < size; i++) {
            result += (long) a[i] * b[i];
        }
        return result;
    }

    public static double dotProductPlainFloat(float[] a, float[] b, int size) {
        double result = 0;
        for (int i = 0; i < size; i++) {
            result += a[i] * b[i];
        }
        return result;
    }

    public static double dotProductPlainDouble(double[] a, double[] b, int size) {
        double result = 0;
        for (int i = 0; i < size; i++) {
            result += a[i] * b[i];
        }
        return result;
    }

    // Parallel dot product methods
    public static long dotProductParallelByte(byte[] a, byte[] b, int size) {
        // int n_threads = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(n_threads);
        final long[] results = new long[n_threads];
        
        int chunkSize = size / n_threads;
        
        for (int t = 0; t < n_threads; t++) {
            final int threadId = t;
            final int start = t * chunkSize;
            final int end = (t == n_threads - 1) ? size : (t + 1) * chunkSize;
            
            executor.execute(() -> {
                long localSum = 0;
                for (int i = start; i < end; i++) {
                    localSum += a[i] * b[i];
                }
                results[threadId] = localSum;
            });
        }
        
        executor.shutdown();
        try {
            executor.awaitTermination(1, TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        long totalSum = 0;
        for (long partialSum : results) {
            totalSum += partialSum;
        }
        
        return totalSum;
    }

    public static long dotProductParallelShort(short[] a, short[] b, int size) {
        // int n_threads = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(n_threads);
        final long[] results = new long[n_threads];
        
        int chunkSize = size / n_threads;
        
        for (int t = 0; t < n_threads; t++) {
            final int threadId = t;
            final int start = t * chunkSize;
            final int end = (t == n_threads - 1) ? size : (t + 1) * chunkSize;
            
            executor.execute(() -> {
                long localSum = 0;
                for (int i = start; i < end; i++) {
                    localSum += a[i] * b[i];
                }
                results[threadId] = localSum;
            });
        }
        
        executor.shutdown();
        try {
            executor.awaitTermination(1, TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        long totalSum = 0;
        for (long partialSum : results) {
            totalSum += partialSum;
        }
        
        return totalSum;
    }

    public static long dotProductParallelInt(int[] a, int[] b, int size) {
        // int n_threads = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(n_threads);
        final long[] results = new long[n_threads];
        
        int chunkSize = size / n_threads;
        
        for (int t = 0; t < n_threads; t++) {
            final int threadId = t;
            final int start = t * chunkSize;
            final int end = (t == n_threads - 1) ? size : (t + 1) * chunkSize;
            
            executor.execute(() -> {
                long localSum = 0;
                for (int i = start; i < end; i++) {
                    localSum += (long) a[i] * b[i];
                }
                results[threadId] = localSum;
            });
        }
        
        executor.shutdown();
        try {
            executor.awaitTermination(1, TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        long totalSum = 0;
        for (long partialSum : results) {
            totalSum += partialSum;
        }
        
        return totalSum;
    }

    public static double dotProductParallelFloat(float[] a, float[] b, int size) {
        // int n_threads = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(n_threads);
        final double[] results = new double[n_threads];
        
        int chunkSize = size / n_threads;
        
        for (int t = 0; t < n_threads; t++) {
            final int threadId = t;
            final int start = t * chunkSize;
            final int end = (t == n_threads - 1) ? size : (t + 1) * chunkSize;
            
            executor.execute(() -> {
                double localSum = 0;
                for (int i = start; i < end; i++) {
                    localSum += a[i] * b[i];
                }
                results[threadId] = localSum;
            });
        }
        
        executor.shutdown();
        try {
            executor.awaitTermination(1, TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        double totalSum = 0;
        for (double partialSum : results) {
            totalSum += partialSum;
        }
        
        return totalSum;
    }

    public static double dotProductParallelDouble(double[] a, double[] b, int size) {
        // int n_threads = Runtime.getRuntime().availableProcessors();
        ExecutorService executor = Executors.newFixedThreadPool(n_threads);
        final double[] results = new double[n_threads];
        
        int chunkSize = size / n_threads;
        
        for (int t = 0; t < n_threads; t++) {
            final int threadId = t;
            final int start = t * chunkSize;
            final int end = (t == n_threads - 1) ? size : (t + 1) * chunkSize;
            
            executor.execute(() -> {
                double localSum = 0;
                for (int i = start; i < end; i++) {
                    localSum += a[i] * b[i];
                }
                results[threadId] = localSum;
            });
        }
        
        executor.shutdown();
        try {
            executor.awaitTermination(1, TimeUnit.MINUTES);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
        
        double totalSum = 0;
        for (double partialSum : results) {
            totalSum += partialSum;
        }
        
        return totalSum;
    }

    public static void main(String[] args) {
        if (args.length < 3) {
            System.err.println("Usage: java Dotproduct <size> <data_type> <mode>");
            System.err.println("  data_type: char, short, int, float, double");
            System.err.println("  mode: plain, parallel");
            System.exit(1);
        }

        int size = Integer.parseInt(args[0]);
        String dataType = args[1];
        String mode = args.length > 2 ? args[2] : "plain";
        if (mode.equals("parallel")) {
            n_threads = Integer.parseInt(args[3]);
        } else if (!mode.equals("plain")) {
            System.err.println("Invalid mode: " + mode);
            System.exit(1);
        }
        
        random.setSeed(System.currentTimeMillis());
        
        long startTime, endTime;
        double elapsedTime;
        
        switch (dataType) {
            case "char":
                byte[] byteA = createVectorByte(size);
                byte[] byteB = createVectorByte(size);
                randomizeByte(byteA, (byte) -2, (byte) 1);
                randomizeByte(byteB, (byte) -2, (byte) 1);
                
                startTime = System.nanoTime();
                if (mode.equals("parallel")) {
                    dotProductParallelByte(byteA, byteB, size);
                } else {
                    dotProductPlainByte(byteA, byteB, size);
                }
                endTime = System.nanoTime();
                break;
                
            case "short":
                short[] shortA = createVectorShort(size);
                short[] shortB = createVectorShort(size);
                randomizeShort(shortA, (short) -2, (short) 1);
                randomizeShort(shortB, (short) -2, (short) 1);
                
                startTime = System.nanoTime();
                if (mode.equals("parallel")) {
                    dotProductParallelShort(shortA, shortB, size);
                } else {
                    dotProductPlainShort(shortA, shortB, size);
                }
                endTime = System.nanoTime();
                break;
                
            case "int":
                int[] intA = createVectorInt(size);
                int[] intB = createVectorInt(size);
                randomizeInt(intA, -2, 1);
                randomizeInt(intB, -2, 1);
                
                startTime = System.nanoTime();
                if (mode.equals("parallel")) {
                    dotProductParallelInt(intA, intB, size);
                } else {
                    dotProductPlainInt(intA, intB, size);
                }
                endTime = System.nanoTime();
                break;
                
            case "float":
                float[] floatA = createVectorFloat(size);
                float[] floatB = createVectorFloat(size);
                randomizeFloat(floatA, -2.0f, 1.0f);
                randomizeFloat(floatB, -2.0f, 1.0f);
                
                startTime = System.nanoTime();
                if (mode.equals("parallel")) {
                    dotProductParallelFloat(floatA, floatB, size);
                } else {
                    dotProductPlainFloat(floatA, floatB, size);
                }
                endTime = System.nanoTime();
                break;
                
            case "double":
                double[] doubleA = createVectorDouble(size);
                double[] doubleB = createVectorDouble(size);
                randomizeDouble(doubleA, -2.0, 1.0);
                randomizeDouble(doubleB, -2.0, 1.0);
                
                startTime = System.nanoTime();
                if (mode.equals("parallel")) {
                    dotProductParallelDouble(doubleA, doubleB, size);
                } else {
                    dotProductPlainDouble(doubleA, doubleB, size);
                }
                endTime = System.nanoTime();
                break;
                
            default:
                System.err.println("Unsupported data type: " + dataType);
                System.exit(1);
                return; // Just to make the compiler happy
        }
        
        elapsedTime = (endTime - startTime) / 1_000_000_000.0; // Convert to seconds
        System.out.printf("%.9f\n", elapsedTime);
    }
}
