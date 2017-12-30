package ins.developers.indoornavigationsystem;

import org.junit.Test;
import static org.junit.Assert.*;

public class MainActivityUnitTest {
    @Test
    public void mainActivity_whenCalled_willHaveCorrectLogTag() throws Exception {
        MainActivity mainActivity = new MainActivity();
        final String expectedLogTag = mainActivity.getClass().getSimpleName();
        final String actualLogTag = mainActivity.TAG;
        assertEquals(expectedLogTag, actualLogTag);
    }
}