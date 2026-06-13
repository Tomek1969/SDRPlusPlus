import androidx.compose.foundation.Canvas
import androidx.compose.foundation.background
import androidx.compose.foundation.gestures.detectDragGestures
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.size
import androidx.compose.runtime.*
import androidx.compose.ui.Modifier
import androidx.compose.ui.geometry.Offset
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.input.pointer.pointerInput
import androidx.compose.ui.unit.dp
import kotlin.math.atan2
import kotlin.math.cos
import kotlin.math.sin

@Composable
fun VfoKnob(
    modifier: Modifier = Modifier,
    onFrequencyChanged: (Double) -> Unit // Callback przekazujący zmianę w Hz
) {
    var angle by remember { mutableStateOf(0f) }
    val knobRadius = 100.dp
    
    Box(
        modifier = modifier
            .size(knobRadius * 2)
            .background(Color.Transparent)
            .pointerInput(Unit) {
                detectDragGestures { change, dragAmount ->
                    change.consume()
                    
                    // Środek pokrętła w pikselach
                    val centerX = size.width / 2f
                    val centerY = size.height / 2f
                    
                    // Aktualna pozycja palca względem środka
                    val currentX = change.position.x - centerX
                    val currentY = change.position.y - centerY
                    
                    // Poprzednia pozycja palca względem środka
                    val prevX = (change.position.x - dragAmount.x) - centerX
                    val prevY = (change.position.y - dragAmount.y) - centerY
                    
                    // Obliczanie kąta przesunięcia palca
                    val currentAngle = atan2(currentY, currentX)
                    val prevAngle = atan2(prevY, prevX)
                    var deltaAngle = currentAngle - prevAngle
                    
                    // Korekta przeskoku kąta (np. z -PI na +PI)
                    if (deltaAngle > Math.PI) deltaAngle -= (2 * Math.PI).toFloat()
                    if (deltaAngle < -Math.PI) deltaAngle += (2 * Math.PI).toFloat()
                    
                    angle += deltaAngle
                    
                    // Przeliczenie obrotu na zmianę częstotliwości (np. 1 stopień = 500 Hz)
                    val deltaHz = (deltaAngle * (180 / Math.PI)) * 500.0
                    onFrequencyChanged(deltaHz)
                }
            }
    ) {
        Canvas(modifier = Modifier.size(knobRadius * 2)) {
            val center = Offset(size.width / 2, size.height / 2)
            val radiusPx = size.width / 2
            
            // 1. Rysowanie głównego korpusu pokrętła (ciemny metalik)
            drawCircle(
                color = Color(0xFF2C2C2C),
                radius = radiusPx,
                center = center
            )
            
            // 2. Rysowanie ramki/obwódki pokrętła
            drawCircle(
                color = Color(0xFF4A4A4A),
                radius = radiusPx - 4f,
                center = center,
                style = androidx.compose.ui.graphics.drawscope.Stroke(width = 4f)
            )
            
            // 3. Rysowanie wskaźnika (kropki) na pokrętle na podstawie aktualnego kąta
            val markerRadius = radiusPx - 30f
            val markerX = center.x + markerRadius * cos(angle)
            val markerY = center.y + markerRadius * sin(angle)
            
            drawCircle(
                color = Color(0xFF00E5FF), // Jasnoniebieski wskaźnik LED
                radius = 12f,
                center = Offset(markerX, markerY)
            )
        }
    }
}
