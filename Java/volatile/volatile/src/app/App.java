package app;



class Aobing extends Thread {

    private boolean flag = false;

    /**
     * @return the flag
     */
    public boolean isFlag() {        
        return flag;
    }

	@Override
	public void run() {
        try {
            Thread.sleep(1000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
       
        flag = true;
        System.out.println("flag =" + flag);
	}
	

}


public class App {
    public static void main(String[] args) throws Exception {
        Aobing a = new Aobing();
        a.start();

        for(;;)
        {
            if(a.isFlag())
            {
                System.out.println("做点什么");
            }
            Thread.sleep(50);
        }
       
    }
}