import { PORT, dbConnect } from './config.js';
import express from 'express';
import cors from 'cors';
import {routes} from './routes/api/index.js';

dbConnect();

// Create an Express app

const app = express();

app.use(cors());

// parse the updates to JSON
app.use(express.json());

app.use(express.urlencoded({ extended: true }));

app.get("/", (req, res) => res.send("Express on Vercel"));

console.log('bot server started...');

app.use("/api", routes);

app.listen(PORT, () => console.log(`Server is running on port ${PORT}`));